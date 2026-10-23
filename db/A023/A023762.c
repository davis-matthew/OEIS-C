/*
 * A023762  Nialpdromes: digits in base 6 are in nonincreasing order.
 * https://oeis.org/A023762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 925, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   Binom, CountLen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A023762 – Nialpdromes: digits in base‑6 are in non‑increasing order.
   The function returns the n‑th term (offset 1).  It uses combinatorics
   to locate the n‑th number without generating all previous elements. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/Binom.h"
#include "../utils/CountLen.h"

/* --- Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety guard – offset is 1 */

    seq_t cumulative = 0;
    idx_t k = 0;

    /* find the length k of the n‑th number */
    while (1) {
        ++k;
        seq_t cnt = CountLen(k);
        if ((seq_t)n <= cumulative + cnt) break;
        cumulative += cnt;
    }

    /* zero‑based index inside the set of k‑digit numbers */
    seq_t pos = (seq_t)n - cumulative - 1;

    if (k == 1) {                /* single–digit numbers 0 … 5 */
        return (seq_t)pos;
    }

    seq_t value = 0;
    idx_t rem = k;
    int cur_max = 5;
    int first_digit = 1;

    /* construct the number digit by digit */
    while (rem > 0) {
        if (first_digit) {
            for (int d = 1; d <= 5; ++d) {
                seq_t ways = Binom(k - 1 + d, d);
                if (pos < ways) {
                    value = value * 6 + d;
                    cur_max = d;
                    --rem;
                    first_digit = 0;
                    break;
                }
                pos -= ways;
            }
        } else {
            for (int d = 0; d <= cur_max; ++d) {
                seq_t ways = Binom(rem - 1 + d, d);
                if (pos < ways) {
                    value = value * 6 + d;
                    cur_max = d;
                    --rem;
                    break;
                }
                pos -= ways;
            }
        }
    }

    return value;
}
