/*
 * A165971  The n-th odd nonprime minus the n-th even nonprime.
 * https://oeis.org/A165971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A014076(n) - A163300(n) * A014076(n) is the n‑th odd
 *            nonprime number (1, 9, 15, …), * A163300(n) is the n‑th even
 *            nonprime number (0, 4, 6, 8, …)
 * Range:     u8 n <= 237, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 86 terms through n = 4732 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for unexpected calls */

    /* Find the n-th odd nonprime */
    idx_t count = 0;
    seq_t cand = (seq_t)1;          /* start with 1, the first odd nonprime */
    while (count < n) {
        if (cand == (seq_t)1 || !is_prime_big(cand)) {
            count++;
            if (count == n) break;   /* we have found the n‑th value */
        }
        cand += (seq_t)2;           /* proceed to the next odd number */
    }
    seq_t odd_val = cand;

    /* Compute the n‑th even nonprime */
    seq_t even_val = (n == 1) ? (seq_t)0 : (seq_t)2 * (seq_t)n;

    return odd_val - even_val;
}
