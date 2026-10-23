/*
 * A030671  Smallest extension of n-th prime which is a square.
 * https://oeis.org/A030671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A030666(A000040(n)) a(n) is the smallest square whose
 *            decimal representation starts with the n-th prime
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 36, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    seq_t prime = (seq_t)get_prime(n);

    /* Search for the minimal square that begins with `prime`. */
    for (idx_t t = 0; ; ++t)
    {
        /* 10^t as seq_t. */
        seq_t pow_t = pow10(t);

        /* Lower bound: p * 10^t  (inclusive). */
        seq_t lower_lim = prime * pow_t;
        seq_t floor_low   = floor_sqrt(lower_lim);
        seq_t lower;
        if (floor_low * floor_low == lower_lim)
            lower = floor_low;
        else
            lower = floor_low + 1;

        /* Upper bound: (p + 1) * 10^t  (exclusive). */
        seq_t upper_lim = (prime + 1) * pow_t - 1;
        seq_t upper = floor_sqrt(upper_lim);

        if (lower > upper)
            continue;  /* No square in this digit range; try next. */

        /* Scan all m in [lower, upper]. */
        for (seq_t m = lower; m <= upper; ++m)
        {
            seq_t x = m * m;
            seq_t left = x / pow_t;   /* Leading digits of x. */
            if (left == prime)
                return x;             /* Found the smallest square. */
        }
        /* In practice this loop will never reach here for valid n. */
    }
}
