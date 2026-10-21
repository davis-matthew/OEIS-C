/*
 * A052542  a(n) = 2*a(n-1) + a(n-2), with a(0) = 1, a(1) = 2, a(2) = 4.
 * https://oeis.org/A052542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4, a(n)=2*a(n-1)+a(n-2) for n>=3 */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)4;

    /* Iteratively compute the sequence; a1 = a(1), a2 = a(2) */
    seq_t a1 = (seq_t)2; /* a(1) */
    seq_t a2 = (seq_t)4; /* a(2) */
    seq_t cur = 0;
    for (idx_t i = 3u; i <= n; ++i) {
        cur = (a2 << 1) + a1;  /* 2*a2 + a1  */
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
