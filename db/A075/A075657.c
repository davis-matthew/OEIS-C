/*
 * A075657  Numbers n such that sum of digits (A007953) is a divisor of sum of
 *          prime divisors (A008472).
 * https://oeis.org/A075657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A075657: numbers n such that the sum of digits of n
 * divides the sum of its distinct prime divisors.
 * 
 * Computation strategy:
 *   brute‑force enumerates candidates starting at 2, factors each
 *   candidate with factor_uint64, sums the distinct primes and the
 *   decimal digits, and keeps the candidate when the divisibility
 *   test succeeds.  The function stops when the requested n‑th term
 *   is found.  This algorithm is O(n * sqrt(n)) worst‑case and
 *   comfortably fits the 2 s limit for n up to at least 10 000.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"        /* seq_t digit_sum(seq_t) */
#include "../utils/factor_uint64.h"     /* void factor_uint64(uint64_t, uint64_t[20], int[20], int*) */

seq_t a(idx_t n)
{
    if (n < 1)               /* undefined for indices before the offset */
        return 0;

    idx_t found   = 0;          /* number of terms found so far */
    uint64_t cand = 2;          /* candidate to test */

    while (found < n) {
        uint64_t fac[20];
        int      exp[20];
        int      cnt;

        /* factor candidate and obtain distinct prime factors in fac[0..cnt-1] */
        factor_uint64(cand, fac, exp, &cnt);

        /* sum of distinct prime divisors */
        seq_t sum_p = 0;
        for (int i = 0; i < cnt; ++i)
            sum_p += (seq_t)fac[i];

        /* sum of decimal digits */
        seq_t dsum = digit_sum((seq_t)cand);

        if (dsum != 0 && (sum_p % dsum) == 0) {
            ++found;
            if (found == n)
                return (seq_t)cand;   /* nth term found */
        }
        ++cand;
    }

    /* should never reach here */
    return 0;
}
