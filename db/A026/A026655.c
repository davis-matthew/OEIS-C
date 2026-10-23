/*
 * A026655  T(n,0) + T(n,1) + ... + T(n,n), T given by A026648.
 * https://oeis.org/A026655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + 2*a(n-4) for n>=4, with a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=10
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of T(n,k) for k=0..n from OEIS A026648, sequence A026655. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)10;

    seq_t a0 = (seq_t)1;  /* a(n-4)  */
    seq_t a1 = (seq_t)2;  /* a(n-3)  */
    seq_t a2 = (seq_t)5;  /* a(n-2)  */
    seq_t a3 = (seq_t)10; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)4 * a2 + (seq_t)2 * a0; /* a(n) */
        a0 = a1; /* shift: a(n-3) becomes new a(n-4) */
        a1 = a2; /* a(n-2) becomes new a(n-3) */
        a2 = a3; /* a(n-1) becomes new a(n-2) */
        a3 = val;/* a(n)  becomes new a(n-1) */
    }
    return a3;
}
