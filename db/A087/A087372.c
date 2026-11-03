/*
 * A087372  Smallest k such that 3^k + 2n is a prime, or 0 if no such k
 *          exists. Index of the smallest power of 3 which when increased by
 *          the n-th even number yields a prime.
 * https://oeis.org/A087372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k such that 3^k + 2*n is a prime.
   Return 0 if no such k exists. */
#include <stdbool.h>
#include <stdint.h>
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
    if (n == 0) return 0;                /* offset 1, guard for n<1 */

    /* If n ≡ 0 (mod 3) then 2*n is a multiple of 3.
       Every candidate 3^k + 2*n (k≥1) is then divisible by 3 and >3,
       so it is composite.  Thus a(n)=0 for n divisible by 3. */
    if ((n % 3) == 0) return 0;

    seq_t twoN = (seq_t)n * 2;

    seq_t pow3 = 1;                      /* hold 3^k */
    const idx_t MAX_K = 2000;            /* empirical bound for the search */

    for (idx_t k = 1; k <= MAX_K; ++k) {
        pow3 *= (seq_t)3;                /* update to 3^k */
        seq_t candidate = pow3 + twoN;   /* 3^k + 2*n */
        if (is_prime_big(candidate)) return (seq_t)k;
    }
    return 0;                            /* no prime within the bound */
}
