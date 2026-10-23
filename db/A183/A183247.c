/*
 * A183247  Number of permutations of 1..2*n+4 with each element displaced by
 *          at least n.
 * https://oeis.org/A183247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 265 a(2) = 1708 a(3) = 13365 a(4) = 114124 a(5) = 998112
 *            a(6) = 8751552 a(7) = 76915200 a(n) = 22*a(n-1) - 157*a(n-2) +
 *            360*a(n-3) (for n > 6)
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is 1‑indexed; return 0 for n = 0 */
        return 0;

    /* 1‑indexed array of the first seven values */
    static const seq_t init_vals[7] = {
        (seq_t)265,
        (seq_t)1708,
        (seq_t)13365,
        (seq_t)114124,
        (seq_t)998112,
        (seq_t)8751552,
        (seq_t)76915200
    };

    if (n <= 7)   /* return a hard‑coded initial term */
        return init_vals[n - 1];

    /* The recurrence needs the three previous terms.
       a1  = a(n-1), a2 = a(n-2), a3 = a(n-3) */
    seq_t a1 = init_vals[6];   /* a(7) */
    seq_t a2 = init_vals[5];   /* a(6) */
    seq_t a3 = init_vals[4];   /* a(5) */

    /* Compute successive terms up to a(n) */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (seq_t)22 * a1 - (seq_t)157 * a2 + (seq_t)360 * a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;
}
