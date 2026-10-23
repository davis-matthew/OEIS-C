/*
 * A176513  a(n+5) = a(n+3) + a(n+2) + a(n), with a(1) = a(2) = a(3) = a(4) =
 *          a(5) = 1.
 * https://oeis.org/A176513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+5) = a(n+3) + a(n+2) + a(n); a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 65, u64 n <= 127,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The term is 1 for the first five indices. */
    if (n <= 5) return (seq_t)1;

    seq_t a1 = (seq_t)1;  /* a(n-5) */
    seq_t a2 = (seq_t)1;  /* a(n-4) */
    seq_t a3 = (seq_t)1;  /* a(n-3) */
    seq_t a4 = (seq_t)1;  /* a(n-2) */
    seq_t a5 = (seq_t)1;  /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = a4 + a3 + a1;   /* a(i) = a(i-2) + a(i-3) + a(i-5) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;   /* a(n) */
}
