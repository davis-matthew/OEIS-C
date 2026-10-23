/*
 * A142717  First (leftmost) odd term in the n-th row of triangle A120070.
 * https://oeis.org/A142717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2-1 if n odd; (n+1)^2-4 if n even
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) defined for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Cast n to seq_t to perform arithmetic in the correct type */
    seq_t nn   = (seq_t)n;
    seq_t n1   = nn + (seq_t)1;
    seq_t sq   = n1 * n1;

    if (n & (idx_t)1)    /* n is odd */
        return sq - (seq_t)1;
    else                 /* n is even */
        return sq - (seq_t)4;
}
