/*
 * A056564  Number of times primes can be produced by taking the absolute
 *          difference between the n-th triangular number and another
 *          triangular number.
 * https://oeis.org/A056564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of primes produced by |T_n – T_k| where T_k ≤ 2T_n
 *            + 3. T_n is the n‑th triangular number T_n = n(n+1)/2
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* triangular number of index n */
    seq_t m = (seq_t)n * ((seq_t)n + 1) / 2;
    /* upper bound for k: largest T_k with T_k ≤ 2*m + 3 */
    seq_t threshold = 2 * m + 3;

    seq_t count = 0;
    seq_t t = 0;            /* T_0 */

    for (idx_t k = 0; ; ++k)
    {
        /* absolute difference between T_n and T_k */
        seq_t diff = (m > t) ? (m - t) : (t - m);
        if (isPrimeGeneric(diff))
            count += 1;

        /* compute next triangular number T_{k+1} = T_k + (k+1) */
        seq_t next_t = t + (seq_t)(k + 1);
        if (next_t > threshold)
            break;
        t = next_t;
    }
    return count;
}
