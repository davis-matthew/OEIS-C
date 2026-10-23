/*
 * A044602  Numbers n such that string 4,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th integer m such that the octal representation of m contains the substring "42" but that of m+1 does not. */
#include <stdlib.h>

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;
static idx_t last_checked = 0;

static int has42(idx_t x)
{
    int prev = -1;          /* previous (lower) octal digit */
    while (x > 0)
    {
        int d = x & 7;        /* x % 8, since base 8 */
        if (d == 4 && prev == 2)
            return 1;        /* found "4,2" in natural order */
        prev = d;
        x >>= 3;            /* x /= 8 */
    }
    return 0;
}

static void ensure_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while (seq_len < n)
    {
        last_checked++;
        if (has42(last_checked) && !has42(last_checked + 1))
        {
            ensure_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
