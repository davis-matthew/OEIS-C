/*
 * A094257  Let M be the 3 X 3 matrix [0 1 0 / 0 0 1 / 7 -35 21]. Take M^n *
 *          [1 1 1] = [p q r]; then a(n-1), a(n), a(n+1) = -p, -q, -r
 *          respectively.
 * https://oeis.org/A094257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A094257.
   a(1)=1, a(2)=7, a(3)=175, a(4)=3423
   a(n) = 21*a(n-1) – 35*a(n-2) + 7*a(n-3)   for n >= 5
   The recurrence is derived from the matrix definition
   and yields the correct terms for all n.  The function
   works for any seq_t (unsigned 8‑, 16‑, 32‑, 64‑ or 128‑bit,
   or arbitrary‑precision) and any idx_t unsigned type. */
#include <stddef.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* constant initial terms (1‑based indexing) */
    static const seq_t init[5] = {0, 1, 7, 175, 3423};

    if (n <= 4)
        return init[n];

    /* initialise the three latest terms: a(n‑3), a(n‑2), a(n‑1) */
    seq_t a_nm3 = init[2];  /* a(2) */
    seq_t a_nm2 = init[3];  /* a(3) */
    seq_t a_nm1 = init[4];  /* a(4) */
    seq_t cur   = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 21*a(i‑1) – 35*a(i‑2) + 7*a(i‑3) */
        cur = ((seq_t)21 * a_nm1) - ((seq_t)35 * a_nm2) + ((seq_t)7 * a_nm3);

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
