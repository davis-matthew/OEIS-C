/*
 * A023105  Number of distinct quadratic residues mod 2^n.
 * https://oeis.org/A023105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2^n + 10)/6)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1; a(1)=2; for n≥2, a(n)=2*a(n-1)-2 if n even,
 * otherwise a(n)=2*a(n-1)-1. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* Iterate using the recurrence */
    seq_t cur = (seq_t)2;        /* a(1) */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        if (i & (idx_t)1)           /* i is odd */
            cur = (seq_t)2 * cur - (seq_t)1;
        else                         /* i is even */
            cur = (seq_t)2 * cur - (seq_t)2;
    }
    return cur;
}
