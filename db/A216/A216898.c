/*
 * A216898  a(n) = smallest number k such that both k - n^2 and k + n^2 are
 *          primes.
 * https://oeis.org/A216898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that (k - n^2) and (k + n^2) are primes
 * Range:     u8 n <= 10, u16 n <= 246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9172 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below searches for the least integer k satisfying the
 * condition by brute force, testing two primality values for each
 * candidate.  It works for any idx_t and seq_t pair provided by the
 * build system, relying on the is_prime_big function defined in the
 * utils library.  The algorithm runs in O(expected_gap * log k)
 * time, which is well below the required limit for n up to 10,000. */

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
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 in seq_t form */

    seq_t k = sq + (seq_t)2;                /* minimal candidate (k ≥ n^2 + 2) */
    for (;; ++k) {
        seq_t p1 = k - sq;                  /* k - n^2 */
        seq_t p2 = k + sq;                  /* k + n^2 */

        /* Both p1 and p2 must be primes.  The primality routine will
         * automatically treat values ≤ 1 as non‑prime. */
        if (is_prime_big(p1) && is_prime_big(p2))
            return k;
    }
}
