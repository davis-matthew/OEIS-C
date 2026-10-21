/*
 * A228604  The Merrifield-Simmons index of the ortho-polyphenylene chain of
 *          length n.
 * https://oeis.org/A228604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=18; a(n)=18*a(n-1)-25*a(n-2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    seq_t a_prevprev = (seq_t)1;  /* a(0) */
    seq_t a_prev     = (seq_t)18; /* a(1) */
    seq_t a_cur;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        a_cur = a_prev * (seq_t)18 - a_prevprev * (seq_t)25;
        a_prevprev = a_prev;
        a_prev = a_cur;
    }
    return a_prev;
}
