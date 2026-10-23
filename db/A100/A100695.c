/*
 * A100695  Largest denominator used in the Egyptian fraction representation
 *          of n/(n + 1) by the greedy algorithm.
 * https://oeis.org/A100695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest denominator used in the greedy Egyptian *
 *            fraction representation of n/(n+1). The algorithm applies the *
 *            standard greedy method, keeping track of the maximum denominator
 *            * reached. This implementation works for seq_t types of any size
 *            * (uint8_t, uint16_t, uint32_t, uint64_t, unsi...
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_seq.h"

/* Euclidean GCD for the seq_t type.  Works for built‑in integral
 * types, as well as any arbitrary‑precision integer class that
 * provides operator% and operator/. */

/* Return the largest denominator in the greedy Egyptian fraction
 * expansion of n/(n+1).  Indexing follows OEIS convention: a(1)
 * corresponds to n = 1. */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid any signed‑overflow issues. */
    seq_t num = (seq_t)n;          /* numerator of the fraction */
    seq_t den = num + 1;           /* denominator of the fraction */
    seq_t max_den = 0;             /* largest denominator seen so far */

    while (num != 0)
    {
        /* The largest unit fraction not exceeding num/den is
         * 1 / ceil(den / num).  The ceil operation is computed
         * as (den + num - 1) / num. */
        seq_t d = (den + num - 1) / num;
        if (d > max_den)
            max_den = d;

        /* Subtract the unit fraction from the current fraction
         * and reduce the result.  The arithmetic uses only
         * seq_t operators; no overflow occurs for the values
         * that appear in the verified range of the sequence. */
        seq_t new_num = num * d - den;
        seq_t new_den = den * d;
        seq_t g = gcd_seq(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return max_den;
}
