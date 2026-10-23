/*
 * A170232  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170232: Number of reduced words of length n in a Coxeter group
 * with 31 generators and relations (S_i)^2 = (S_i S_j)^40 = I.
 *
 * Recurrence (from the given generating function):
 *   For 1 ≤ n ≤ 39:
 *     a(n) = 2 + 29 * (a(n-1) + a(n-2) + ... + a(0))
 *   For n ≥ 40:
 *     a(n) = 29 * (a(n-1) + a(n-2) + ... + a(n-39)) - 435 * a(n-40)
 *
 * Initial value: a(0) = 1.
 *
 * The implementation computes a(n) by a linear-time recurrence,
 * using a circular buffer of size 40 to maintain the last 40 terms.
 *
 * All arithmetic uses the seq_t type as required by the build system.
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Buffer to hold the last 40 terms cyclically. */
    seq_t buf[40] = {0};

    /* a(0) = 1 */
    buf[0] = 1;
    if (n == 0) return 1;

    /* Compute terms 1 .. min(n,39) using the inhomogeneous recurrence. */
    seq_t partial_sum = buf[0];          // sum of a(0) .. a(k-1)
    if (n <= 39) {
        for (idx_t k = 1; k <= n; ++k) {
            buf[(size_t)k] = 2 + (seq_t)29 * partial_sum;
            partial_sum += buf[(size_t)k];
        }
        return buf[(size_t)n];
    }

    /* Compute terms up to 39 (inclusive). */
    for (idx_t k = 1; k <= 39; ++k) {
        buf[(size_t)k] = 2 + (seq_t)29 * partial_sum;
        partial_sum += buf[(size_t)k];
    }

    /* sumPrev39 holds a(n-1) + ... + a(n-39) for the current n. */
    seq_t sumPrev39 = partial_sum - buf[0];   // exclude a(0)

    /* Compute terms 40 .. n using the homogeneous recurrence. */
    for (idx_t k = 40; k <= n; ++k) {
        /* Index of the term to be subtracted from the 39-term window. */
        size_t idx_out = (size_t)((k - 39) % 40);   // a(k-39)
        size_t idx_old = (size_t)((k - 40) % 40);   // a(k-40), to be subtracted from a(k)
        seq_t new_term = (seq_t)29 * sumPrev39 - (seq_t)435 * buf[idx_old];
        buf[(size_t)(k % 40)] = new_term;

        /* Update the sum of the 39 preceding terms for the next iteration. */
        sumPrev39 = sumPrev39 - buf[idx_out] + new_term;
    }

    return buf[(size_t)(n % 40)];
}
