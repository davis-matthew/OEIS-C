/*
 * A089122  Triangle read by rows in which row n gives prime factors of n^2 +
 *          1.
 * https://oeis.org/A089122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = prime factors of n^2 + 1 (distinct), read row‑wise
 * Range:     u8 n <= 25, u16 n <= 589, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the n‑th term of sequence A089122.  n starts at 1.  */
seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is undefined for n=0, but return 0 as safe fallback */
        return 0;

    idx_t cumulative = 0;   /* number of terms produced so far */
    idx_t k = 1;            /* current row (n for which we factor n^2+1) */

    /* We use a fixed array large enough for the number of distinct prime
       factors of n^2+1 for all n up to the limits considered here.  */
    seq_t factors[32];
    size_t factor_count;

    for (;;) {
        /* Compute n^2 + 1 in 64‑bit arithmetic; n <= 2^32 so the value fits. */
        uint64_t val = (uint64_t)k * k + 1;
        uint64_t tmp = val;
        factor_count = 0;

        /* Divide out all powers of 2 (record the prime only once). */
        if (tmp % 2 == 0) {
            factors[factor_count++] = (seq_t)2;
            do { tmp /= 2; } while (tmp % 2 == 0);
        }

        /* Trial division by odd numbers up to sqrt(tmp). */
        for (uint64_t d = 3; d * d <= tmp; d += 2) {
            if (tmp % d == 0) {
                factors[factor_count++] = (seq_t)d;
                do { tmp /= d; } while (tmp % d == 0);
            }
        }
        /* If a prime factor greater than sqrt remains, record it. */
        if (tmp > 1) {
            factors[factor_count++] = (seq_t)tmp;
        }

        /* Check if the desired index lies in this row. */
        if (cumulative + factor_count >= n) {
            size_t pos = (size_t)(n - cumulative - 1);   /* zero‑based */
            return factors[pos];
        }

        cumulative += factor_count;
        ++k;
    }

    /* Should never reach here.  Return 0 to silence compiler warnings. */
    return 0;
}
