/*
 * A234649  Difference between the first members of the widest and the
 *          narrowest prime pair having an arithmetic mean of n.
 * https://oeis.org/A234649
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (largest p < n with p and (2n-p) prime) – (smallest p < n
 *            with p and (2n-p) prime)
 * Range:     u8 n <= 271, u16 n <= 9462, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The equal‑prime pair (p = n) is excluded, as the OEIS definition requires distinct primes. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 8)   /* undefined for offset; return 0 for safety */
        return 0;

    idx_t min_p = 0, max_p = 0;
    /* search all primes p < n, skipping the equal‑prime case p = n */
    for (idx_t p = 2; p < n; ++p) {
        if (!is_prime(p))
            continue;
        idx_t q = 2 * n - p;
        if (!is_prime(q))
            continue;

        if (min_p == 0 || p < min_p)
            min_p = p;
        if (max_p == 0 || p > max_p)
            max_p = p;
    }

    if (min_p == 0)   /* no suitable pair found (should not happen for n ≥ 8) */
        return 0;

    return (seq_t)(max_p - min_p);
}
