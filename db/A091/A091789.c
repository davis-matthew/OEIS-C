/*
 * A091789  a(1) = 1, a(2) = 2, a(n) = reverse concatenation of two previous
 *          terms.
 * https://oeis.org/A091789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = reverse(a(n‑1)) concatenated with
 *            reverse(a(n‑2))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>          /* for uint64_t (idx_t) */
#include <stddef.h>          /* for size_t */

#include "../utils/pow10.h"    /* seq_t pow10(idx_t e);            */
#include "../utils/digit_len.h"/* int  digit_len(seq_t x);          */

/* a(1) = 1, a(2) = 2, a(n) = rev(a(n-1)) || rev(a(n-2))   */
seq_t a(idx_t n)
{
    static seq_t *values      = NULL;  /* a(i)                */
    static seq_t *rev_values  = NULL;  /* rev(a(i))           */
    static idx_t  capacity   = 0;      /* allocated size      */
    static idx_t  last_idx   = 0;      /* last computed index */

    /* n == 0 is out of range; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* grow storage if necessary */
    if (capacity <= n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp_vals = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        seq_t *tmp_rev  = (seq_t *)realloc(rev_values, new_cap * sizeof(seq_t));
        if (!tmp_vals || !tmp_rev)
            abort();                      /* memory exhausted */
        values      = tmp_vals;
        rev_values  = tmp_rev;
        capacity    = new_cap;
    }

    /* compute missing terms in order */
    for (idx_t i = last_idx + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            values[1]    = (seq_t)1;
            rev_values[1] = (seq_t)1;
        }
        else if (i == 2)
        {
            values[2]    = (seq_t)2;
            rev_values[2] = (seq_t)2;
        }
        else
        {
            /* a(i) = rev(a(i-1)) || rev(a(i-2)) */
            int len_ai2  = digit_len(values[i-2]);   /* same as len(rev of a(i-2)) */
            seq_t part1  = rev_values[i-1] * pow10(len_ai2);
            values[i]    = part1 + rev_values[i-2];

            /* rev(a(i)) = a(i-2) || a(i-1) */
            int len_ai1  = digit_len(values[i-1]);   /* length of a(i-1) */
            seq_t rev_part1 = values[i-2] * pow10(len_ai1);
            rev_values[i]    = rev_part1 + values[i-1];
        }
    }

    last_idx = n;
    return values[n];
}
