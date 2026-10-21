/*
 * A069571  Numbers n in which the k-th digit (counted from the right) is
 *          nonzero and is either a divisor of k (but not 1 in case k has a
 *          single-digit prime divisor) or a multiple of k, for all 1 <= k <=
 *          number of digits of n.
 * https://oeis.org/A069571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   allowed_digits_for_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the nth term of OEIS A069571
   by enumerating all admissible decimal numbers digit‑by‑digit.
   The digits are chosen from left to right using positional
   radices defined by the constraints in the definition. */

#include <stdbool.h>
#include "../utils/allowed_digits_for_pos.h"

seq_t a(idx_t n)
{
    /* Find the length L of the desired number (number of digits). */
    idx_t cumulative = 0;
    idx_t product    = 1;
    int   len        = 0;

    /* Store the count of admissible digits for each position up to len.
       1‑based indexing is used for convenience. */
    int counts[64] = {0};

    while (cumulative < n) {
        ++len;
        int digits[9], cnt;
        allowed_digits_for_pos(len, digits, &cnt);
        counts[len] = cnt;
        product = product * (idx_t)cnt;
        cumulative += product;
        if (len >= 63) break;                /* safety guard, not needed for tested n */
    }

    /* Number of terms with fewer than len digits. */
    idx_t cumulative_prev = cumulative - product;
    /* Index of the desired number within the len‑digit block (0‑based). */
    idx_t index = n - cumulative_prev - 1;

    /* Prefix products: prefix[i] = product of counts for positions 1..i. */
    idx_t prefix[65];
    prefix[0] = 1;
    for (int i = 1; i <= len; ++i)
        prefix[i] = prefix[i - 1] * (idx_t)counts[i];

    seq_t result = 0;
    for (int pos = len; pos >= 1; --pos) {
        int digits[9], cnt;
        allowed_digits_for_pos(pos, digits, &cnt);

        idx_t subcnt = prefix[pos - 1];   /* combinations for lower positions */

        for (int i = 0; i < cnt; ++i) {
            int d = digits[i];
            if (index >= subcnt) {
                index -= subcnt;
            } else {
                result = result * (seq_t)10 + (seq_t)d;
                break;
            }
        }
    }
    return result;
}
