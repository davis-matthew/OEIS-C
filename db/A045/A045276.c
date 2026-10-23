/*
 * A045276  Numbers whose base-5 representation contains exactly three 2's and
 *          two 3's.
 * https://oeis.org/A045276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-5 representation contains exactly three 2's
   and two 3's (OEIS A045276).  The function builds the sequence on
   demand and returns the nth term (1‑based). */

#include <stdlib.h>
#include <stddef.h>

/* static storage for the generated sequence */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_len = 5;   /* next length to generate */

/* ensure capacity in the array */
static void add_number(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* recursive generator for a fixed length */
static void gen_rec(idx_t pos, idx_t len,
                    int rem2, int rem3,
                    seq_t val)
{
    if (pos == len) {
        add_number(val);
        return;
    }

    int pos_left = (int)(len - pos - 1);

    for (int d = 0; d < 5; ++d) {
        if (pos == 0 && d == 0)
            continue;                /* no leading zero */

        if (d == 2) {
            if (rem2 == 0)
                continue;
            if (rem2 - 1 + rem3 > pos_left)
                continue;
            gen_rec(pos + 1, len, rem2 - 1, rem3,
                    val * 5 + (seq_t)d);
        } else if (d == 3) {
            if (rem3 == 0)
                continue;
            if (rem2 + rem3 - 1 > pos_left)
                continue;
            gen_rec(pos + 1, len, rem2, rem3 - 1,
                    val * 5 + (seq_t)d);
        } else { /* other digits 0,1,4 */
            if (rem2 + rem3 > pos_left)
                continue;
            gen_rec(pos + 1, len, rem2, rem3,
                    val * 5 + (seq_t)d);
        }
    }
}

/* extend the generated sequence until it contains at least n terms */
static void extend_to(idx_t n)
{
    while (seq_len < n) {
        gen_rec(0, next_len, 3, 2, 0);
        ++next_len;
    }
}

/* externally visible function */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but keep safe */
        return (seq_t)0;
    if (seq_len < (size_t)n)
        extend_to(n);
    return seq[n - 1];
}
