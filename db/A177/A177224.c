/*
 * A177224  a(n) is the least prime of the form ((2n+1)^k - (2n-1)^k)/2.
 * https://oeis.org/A177224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = the least prime of the form ((2n+1)^k - (2n-1)^k)/2 where
 *            k is an odd prime. The implementation tests odd primes k in
 *            ascending order until the corresponding value is prime
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* n is at least 1 (OEIS offset 1). */
    seq_t base1 = ((seq_t)2 * (seq_t)n) + 1;   /* 2n + 1  */
    seq_t base2 = ((seq_t)2 * (seq_t)n) - 1;   /* 2n - 1  */

    uint64_t k = 3;                            /* first odd prime */
    while (1) {
        seq_t p1 = ipow_idx(base1, (idx_t)k);
        seq_t p2 = ipow_idx(base2, (idx_t)k);
        seq_t val = (p1 - p2) >> 1;            /* (a^k - b^k) / 2 */

        if (is_prime_big(val))
            return val;

        k = next_prime(k + 1);                 /* next odd prime */
    }
}
