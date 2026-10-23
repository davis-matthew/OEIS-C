/*
 * A179611  Eight white kings and one red king on a 3 X 3 chessboard. G.f.:
 *          (1+2*x)/(1 - 2*x - 8*x^2 - 4*x^3).
 * https://oeis.org/A179611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A179611: a(n) = 2*a(n-1) + 8*a(n-2) + 4*a(n-3) 
 * Initial values: a(0)=1, a(1)=4, a(2)=16 
 */
seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    /* iterate using only the last three values (O(n) time, O(1) space) */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)16;  /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 2 * a2 + 8 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
