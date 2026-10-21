/*
 * A099277  Unsigned member r=-19 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - a(n-2) + 2*(-1)^(n+1), a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t prev = (seq_t)0;   /* a(0) */
    seq_t curr = (seq_t)1;   /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t next = (seq_t)21 * curr - prev;
        /* 2*(-1)^(i+1): add 2 when (i+1) is even, subtract 2 when odd */
        if (((i + (idx_t)1) & (idx_t)1) == (idx_t)0)
            next += (seq_t)2;
        else
            next -= (seq_t)2;
        prev = curr;
        curr = next;
    }
    return curr;
}
