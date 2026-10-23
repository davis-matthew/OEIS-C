/*
 * A041176  Numerators of continued fraction convergents to sqrt(98).
 * https://oeis.org/A041176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of convergents to √98.
   a(0)=9, a(1)=10, a(2)=89, a(3)=99, a(4)=1871, ...
   The continued fraction of √98 is [9; 1,8,1,18,1,8,1,18,...] */

/* a(n) = numerator of the nth convergent to sqrt(98) */
seq_t a(idx_t n)
{
    /* periodic part of the continued fraction after the initial term 9 */
    const idx_t period[4] = { (idx_t)1, (idx_t)8, (idx_t)1, (idx_t)18 };

    seq_t prev_prev = 0;   /* p_{-2} */
    seq_t prev      = 1;   /* p_{-1} */
    seq_t curr;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k = (k == 0) ? (seq_t)9 : (seq_t)period[(k - 1) & 3];
        curr = a_k * prev + prev_prev;
        prev_prev = prev;
        prev = curr;
    }
    return prev;
}
