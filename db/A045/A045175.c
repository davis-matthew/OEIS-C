/*
 * A045175  Numbers whose base-5 representation contains no 0's and exactly
 *          three 2's.
 * https://oeis.org/A045175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-5 representation contains no 0's and exactly
 *            three 2's
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

static idx_t pow3(idx_t e)
{
    idx_t r = 1;
    while (e-- > 0) r *= 3;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for 0, return 0 */

    idx_t len = 3;                      /* minimum length (222_5) */
    idx_t remaining = n;

    /* Find the length that contains the n‑th term */
    for (;; ++len) {
        idx_t total = comb(len, 3) * pow3(len - 3);
        if (remaining > total) {
            remaining -= total;
            continue;
        }
        /* remaining is the 1‑based position within numbers of this length */
        idx_t k = remaining - 1;       /* 0‑based rank */
        seq_t value = 0;
        idx_t used_two = 0;

        for (idx_t pos = 0; pos < len; ++pos) {
            idx_t rem = len - pos - 1;
            idx_t rem_two_before = 3 - used_two;

            /* Try digits 1..4 in ascending order */
            for (int digit = 1; digit <= 4; ++digit) {
                /* Determine remaining twos after choosing this digit */
                idx_t new_rem_two = rem_two_before;
                if (digit == 2) {
                    if (used_two == 3) continue; /* too many twos already */
                    new_rem_two = rem_two_before - 1;
                }
                if (rem < new_rem_two) continue; /* insufficient places left */

                /* Count how many numbers start with this digit */
                idx_t combos = comb(rem, new_rem_two) * pow3(rem - new_rem_two);

                if (k >= combos) {
                    k -= combos;              /* skip these */
                    continue;
                }

                /* Choose this digit */
                value = value * (seq_t)5 + (seq_t)digit;
                if (digit == 2) used_two++;
                break; /* move to next position */
            }
        }
        return value;
    }
}
