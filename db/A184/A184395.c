/*
 * A184395  a(n) = number of distinct values obtained when sigma is applied to
 *          the divisors of n.
 * https://oeis.org/A184395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of σ(d) for all divisors d of n
 * Range:     u8 n <= 1406, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided sigma_of utility. */

#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Computes a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* 0 is not a valid index in OEIS, but handle it gracefully. */
    if (n == 0) return 0;

    /* Temporary storage for distinct sigma values. */
    /* The number of divisors of n <= 64 for n <= 10^4, so 128 is safe. */
    seq_t seen[128];
    size_t seen_len = 0;

    /* Enumerate divisors by looping up to sqrt(n). */
    for (idx_t d = 1; (d * d) <= n; ++d)
    {
        if (n % d != 0) continue;

        /* Process divisor d. */
        seq_t s_d = sigma_of(d);
        bool dup = false;
        for (size_t i = 0; i < seen_len; ++i)
        {
            if (seen[i] == s_d) { dup = true; break; }
        }
        if (!dup) seen[seen_len++] = s_d;

        /* Process paired divisor n/d if distinct. */
        idx_t other = n / d;
        if (other != d)
        {
            seq_t s_o = sigma_of(other);
            dup = false;
            for (size_t i = 0; i < seen_len; ++i)
            {
                if (seen[i] == s_o) { dup = true; break; }
            }
            if (!dup) seen[seen_len++] = s_o;
        }
    }

    return (seq_t)seen_len;
}
