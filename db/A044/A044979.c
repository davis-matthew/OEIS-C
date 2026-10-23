/*
 * A044979  Numbers whose base-3 representation contains exactly two 0's and
 *          four 1's.
 * https://oeis.org/A044979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   comb_u64, count_length, count_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044979:
 *   Numbers whose base‑3 representation contains exactly two 0's and four 1's.
 *   Offset 1.  The function enumerates all base‑3 strings satisfying the
 *   condition and returns the n‑th value in increasing numerical order.
 *   Implementation uses combinatorial ranking/unranking.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/comb_u64.h"
#include "../utils/count_length.h"
#include "../utils/count_seq.h"

/* Unrank nth (1‑based) string satisfying the constraints and
 * convert it to its numeric value. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* Find the length L of the base‑3 number that contains the nth string. */
    idx_t cumulative = 0;
    idx_t L = 6;
    for (;; ++L)
    {
        idx_t f = count_length(L);
        if (cumulative + f >= n)
            break;
        cumulative += f;
    }

    /* Index k of the string among all strings of length L (1‑based). */
    idx_t k = n - cumulative;

    unsigned char digits[64] = {0};
    idx_t z = 2;  /* zeros remaining */
    idx_t o = 4;  /* ones remaining */

    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t rem = L - pos;  /* positions remaining including this one */

        for (int d = 0; d <= 2; ++d)
        {
            if (pos == 0 && d == 0)
                continue;                 /* no leading zero */
            if (d == 0 && z == 0)
                continue;                 /* no zeros left */
            if (d == 1 && o == 0)
                continue;                 /* no ones left */

            idx_t new_z = z;
            idx_t new_o = o;
            if (d == 0)
                new_z--;
            else if (d == 1)
                new_o--;

            idx_t branch = count_seq(rem - 1, new_z, new_o);
            if (branch == 0)
                continue;

            if (k > branch)
            {
                k -= branch;
            }
            else
            {
                digits[pos] = (unsigned char)d;
                z = new_z;
                o = new_o;
                break;
            }
        }
    }

    /* Convert the base‑3 digits to a numeric value. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        value = value * (seq_t)3 + (seq_t)digits[i];
    }
    return value;
}
