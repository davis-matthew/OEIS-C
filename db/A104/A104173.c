/*
 * A104173  a(n) is the smallest integer equal to the sum and the product of
 *          the same n positive integers: a(n) = i(1) + i(2) + ... + i(n) =
 *          i(1)*i(2)*...*i(n).
 * https://oeis.org/A104173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest positive integer that can be expressed as
 *            the sum and the product of the same n positive integers
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9257 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Recursive helper that tries to build factorizations of k (target_k) using
   factors >= start_factor. depth is the number of factors >1 already chosen,
   sum_so_far is the sum of these factors. Returns true as soon as a
   suitable factorization for the given n is found. */
static bool search_factors(seq_t target_k,
                           seq_t k_rem,
                           seq_t start_factor,
                           idx_t n,
                           idx_t depth,
                           seq_t sum_so_far)
{
    if (k_rem == 1) {
        /* All factors >1 have been chosen. */
        idx_t r = depth;                     /* number of factors >1 */
        seq_t ones = (seq_t)(n - r);         /* remaining ones */
        if (sum_so_far + ones == target_k)
            return true;
        return false;
    }

    /* Try all possible next factors f. */
    for (seq_t f = start_factor; f * f <= k_rem; ++f) {
        if (k_rem % f == 0) {
            if (depth + 1 > n) /* cannot exceed total count */
                continue;
            if (search_factors(target_k, k_rem / f, f, n, depth + 1,
                               sum_so_far + f))
                return true;
        }
    }

    /* Take k_rem itself as the last factor. */
    if (depth + 1 <= n) {
        seq_t sum_with_last = sum_so_far + k_rem;
        seq_t ones = (seq_t)(n - (depth + 1));
        if (sum_with_last + ones == target_k)
            return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition. */
    if (n == 1)
        return (seq_t)1;              /* the only possibility is {1} */

    /* Search k from n upwards (lower bound) up to the known upper bound 2n. */
    for (idx_t i = n; i <= 2 * n; ++i) {
        seq_t k = (seq_t)i;
        if (search_factors(k, k, 2, n, 0, 0))
            return k;                /* minimal k found */
    }

    /* Theoretical guarantee a(n) <= 2n ensures we never reach here. */
    return (seq_t)0; /* placeholder; should not happen */
}
