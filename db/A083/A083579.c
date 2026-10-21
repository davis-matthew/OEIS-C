/*
 * A083579  Generalized Jacobsthal numbers.
 * https://oeis.org/A083579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=0, a(1)=1, 
 * a(n) = a(n-1) + 2*a(n-2) + (n-2) for n >= 2
 * (equivalently a(n+2)=a(n+1)+2*a(n)+n)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = 0;              /* a(n-2)  */
    seq_t a1 = 1;              /* a(n-1)  */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 + (a0 << 1) + (seq_t)(i - 2);
        a0  = a1;
        a1  = ai;
    }
    return a1;
}
