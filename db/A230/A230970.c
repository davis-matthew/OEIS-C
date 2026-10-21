/*
 * A230970  Number of (n+2) X (1+2) 0..3 arrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A230970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=16, a(3)=64, a(4)=256,
 *            a(n)=4*a(n-1)+20*a(n-3)+25*a(n-4) for n>4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A230970( n ) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)64;
    if (n == 4) return (seq_t)256;

    seq_t a_m4 = (seq_t)0;   /* a(n-4)   */
    seq_t a_m3 = (seq_t)16;  /* a(n-3)   */
    seq_t a_m2 = (seq_t)64;  /* a(n-2)   */
    seq_t a_m1 = (seq_t)256; /* a(n-1)   */
    seq_t ai    = a_m1;      /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        ai = 4 * a_m1 + 20 * a_m3 + 25 * a_m4;
        /* shift window for next iteration */
        a_m4 = a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = ai;
    }
    return ai;
}
