/*
 * A108942  Degrees of irreducible representations of SL(2,7).
 * https://oeis.org/A108942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Degrees of irreducible representations of SL(2,7) */
/*  a(1)=1, a(2)=3, a(3)=3, a(4)=4, a(5)=4, a(6)=6,
    a(7)=6, a(8)=6, a(9)=7, a(10)=8, a(11)=8.  */

/* Return the n-th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    static const seq_t tbl[] = {
        1, 3, 3, 4, 4, 6, 6, 6, 7, 8, 8
    };
    if (n < 1)
        return (seq_t)0;

    idx_t len = sizeof(tbl)/sizeof(tbl[0]);
    if (n <= len)
        return tbl[n-1];
    return (seq_t)0;   /* terms beyond the longest known a(n) are 0 */
}
