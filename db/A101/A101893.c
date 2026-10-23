/*
 * A101893  a(n) = Sum_{k=0..floor(n/2)} C(n,2k)*Pell(k).
 * https://oeis.org/A101893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4) initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly. */
    if (n == 0u) return 0;
    if (n == 1u) return 0;
    if (n == 2u) return 1;
    if (n == 3u) return 3;

    /* a0 … a3 store a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t a0 = 0;   /* a(0)   */
    seq_t a1 = 0;   /* a(1)   */
    seq_t a2 = 1;   /* a(2)   */
    seq_t a3 = 3;   /* a(3)   */
    seq_t an = 0;   /* current value to compute */

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - 4 * a2;         /* 4*a(n-1) - 4*a(n-2), non‑negative   */
        an += 2 * a0;                 /* + 2*a(n-4)                           */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
