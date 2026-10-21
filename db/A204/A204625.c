/*
 * A204625  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A204625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 60*a(n-1) - 175*a(n-2) + 68*a(n-3) with a(1) = 768, a(2)
 *            = 43734, a(3) = 2490558
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)      /* out‑of‑bounds: behaviour not required */
        return (seq_t)0;

    if (n == 1) return (seq_t)768;
    if (n == 2) return (seq_t)43734;
    if (n == 3) return (seq_t)2490558;

    seq_t a0 = (seq_t)768;      /* a(n-3) */
    seq_t a1 = (seq_t)43734;    /* a(n-2) */
    seq_t a2 = (seq_t)2490558;  /* a(n-1) */
    seq_t a3;                   /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a3 = (seq_t)60 * a2 - (seq_t)175 * a1 + (seq_t)68 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
