/*
 * A118445  Number of tree-rooted maps of genus 1 with n edges: rooted maps on
 *          the torus with a distinguished spanning tree.
 * https://oeis.org/A118445
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118445
 * Formula: a(n) = n*(n-1) * C(2n,n)^2 / (24*(n+1))
 *
 * Recurrence (valid for n ≥ 3):
 *   a(n) = 4*(2n-1)^2 / ((n+1)*(n-2)) * a(n-1)
 *   a(2) = 1
 *
 *  The recurrence is used with exact integer arithmetic;
 *  intermediate cancellations are done with gcd to avoid
 *  overflow on 64‑bit builds.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* The sequence is defined for n ≥ 2 */
        return 0;
    }

    /* Base case a(2) = 1 */
    seq_t a_val = 1;

    /* Iterate using the recurrence from n = 3 up to the desired n */
    for (idx_t k = 3; k <= n; ++k) {
        /* numerator = 4 * (2k-1)^2 */
        seq_t two_k_minus1 = (seq_t)2 * (seq_t)k - 1;
        seq_t num = (seq_t)4 * two_k_minus1 * two_k_minus1;  /* small product */

        /* denominator = (k+1) * (k-2) */
        seq_t den = (seq_t)(k + 1) * (seq_t)(k - 2);

        /* Reduce using gcd to keep intermediate results small */
        seq_t g1 = gcd(a_val, den);
        a_val = a_val / g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* After reduction den should be 1 */
        a_val = a_val * num;
    }

    return a_val;
}
