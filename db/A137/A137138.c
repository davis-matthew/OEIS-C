/*
 * A137138  Numbers k such that k and k^2 use only the digits 3, 6, 8 and 9.
 * https://oeis.org/A137138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   numbers k such that k and k^2 use only digits 3,6,8,9 (generated
 *            by exhaustive search over all decimal numbers composed of the
 *            digits {3,6,8,9})
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return true iff every decimal digit of x belongs to {3,6,8,9}. */
static bool digits_allowed(seq_t x)
{
    do
    {
        seq_t d = x % 10;
        if (d != 3 && d != 6 && d != 8 && d != 9)
            return false;
        x /= 10;
    } while (x != 0);
    return true;
}

/* Recursively generate all numbers of a given length composed of
   the digits 3,6,8,9. For each number, if its square contains only
   those digits, store it in the output vector. */
static void gen_len_recurse(idx_t pos, idx_t len, seq_t cur,
                            seq_t **out, size_t *cnt, size_t *cap)
{
    if (pos == len)
    {
        seq_t sq = cur * cur;
        if (digits_allowed(sq))
        {
            if (*cnt >= *cap)
            {
                *cap = (*cap == 0) ? 8 : (*cap << 1);
                *out = (seq_t*)realloc(*out, *cap * sizeof(seq_t));
            }
            (*out)[*cnt] = cur;
            (*cnt)++;
        }
        return;
    }

    static const uint8_t dig[4] = {3, 6, 8, 9};
    for (int i = 0; i < 4; ++i)
    {
        gen_len_recurse(pos + 1, len, cur * 10 + dig[i],
                        out, cnt, cap);
    }
}

/* Return the n-th term of A137138 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;       /* undefined below offset 1 */

    static seq_t *values = NULL;
    static size_t count  = 0;
    static size_t capacity = 0;
    static idx_t next_length = 1;

    while (count < (size_t)n)
    {
        gen_len_recurse(0, next_length, (seq_t)0,
                        &values, &count, &capacity);
        next_length++;
    }

    return values[n - 1];
}
