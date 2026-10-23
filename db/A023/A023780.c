/*
 * A023780  Metadromes: digits in base 12 are in strict ascending order.
 * https://oeis.org/A023780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 898, u32 n <= 2036, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   init_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer whose digits in base 12 are in strict ascending order.
   The sequence starts with 0 (n=1) and then all non‑zero such integers.
*/
#include <stdint.h>
#include "../utils/init_binom.h"

seq_t a(idx_t n)
{
    /* Handle the trivial first term. */
    if (n == 1) return 0;

    static int binom_inited = 0;
    static idx_t binom[12][12];
    if (!binom_inited) {
        init_binom(binom);
        binom_inited = 1;
    }

    /* Reduce to index among non‑empty subsets of {1..11}. */
    idx_t idx = n - 1;               /* 1‑based index among non‑zero terms */
    idx_t rem = idx;                 /* remaining index to locate */
    idx_t L = 0;                     /* number of digits */

    /* Find the length L of the target number. */
    for (L = 1; L <= 11; ++L)
    {
        idx_t count = binom[11][L];
        if (rem > count) {
            rem -= count;
        } else {
            break;
        }
    }

    /* Unrank the rem‑th combination of L digits from 1..11 in lexicographic order. */
    idx_t digits[11];
    idx_t min_digit = 1;
    idx_t remain = rem - 1;          /* make zero‑based */

    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t max_digit = 11 - (L - 1 - pos);
        for (idx_t d = min_digit; d <= max_digit; ++d)
        {
            idx_t remaining_digits = 11 - d;
            idx_t choose = binom[remaining_digits][L - 1 - pos];
            if (remain < choose) {
                digits[pos] = d;
                min_digit = d + 1;
                break;
            } else {
                remain -= choose;
            }
        }
    }

    /* Convert the digit sequence to base‑12 integer. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * 12 + digits[i];

    return value;
}
