/*
 * A057687  Trajectory of 29 under the `29x+1' map: x -> x/p if p = least
 *          prime factor(x) < 29, else x*29 + 1.
 * https://oeis.org/A057687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 29 * a(n) = a(n-1) / p if the least prime factor p of
 *            a(n-1) satisfies p < 29 * a(n) = 29 * a(n-1) + 1 otherwise *
 *            (for a(n-1) = 1 use the second branch) * For n > 42: a(n) =
 *            a(n-4) (the sequence enters a 4‑cycle) * * The program
 *            precomputes the first 43 terms (indices 0…42) and then * a...
 * Range:     u8 n <= 0, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* The primes less than 29, in ascending order. */
static const uint8_t PRIMES_LT29[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};

/* Return the A057687 term a(n).  The function memoises the first
 * 43 terms and then uses the 4‑cycle for n > 42. */
seq_t
a(idx_t n)
{
    /* The cache holds a(0) … a(42).  It is filled once and reused. */
    static seq_t cache[43];
    static int computed = 0;

    if (!computed) {
        /* Initial term. */
        cache[0] = (seq_t)29;

        /* Compute the next 42 terms using the definition. */
        for (size_t i = 1; i < 43; ++i) {
            seq_t x = cache[i - 1];
            uint8_t p = 0;                     /* least prime factor < 29 */
            for (size_t k = 0; k < sizeof(PRIMES_LT29); ++k) {
                if (x % PRIMES_LT29[k] == 0) {
                    p = PRIMES_LT29[k];
                    break;
                }
            }

            if (p != 0) {
                cache[i] = x / (seq_t)p;       /* divide by the lpf */
            } else {
                /* Use a wider type to avoid intermediate overflow. */
                unsigned long long tmp = (unsigned long long)x * 29 + 1;
                cache[i] = (seq_t)tmp;         /* 29*x + 1 */
            }
        }
        computed = 1;
    }

    if (n <= 42) {
        return cache[n];
    } else {
        /* After index 42 the sequence repeats every 4 terms. */
        return cache[39 + (n - 39) % 4];
    }
}
