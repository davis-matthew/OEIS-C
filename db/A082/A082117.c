/*
 * A082117  Fibonacci sequence (mod 6).
 * https://oeis.org/A082117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 6 (F(0)=0, F(1)=1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fibonacci modulo 6 is periodic with period 24.  */
    idx_t m = n % 24;                /* only the remainder matters   */
    seq_t f0 = 0;                    /* F(0)                       */
    seq_t f1 = 1;                    /* F(1)                       */

    for (idx_t i = 0; i < m; ++i) {
        seq_t nxt = f0 + f1;         /* next Fibonacci number          */
        if (nxt >= 6)                /* reduce modulo 6 explicitly     */
            nxt -= 6;
        f0 = f1;
        f1 = nxt;
    }
    return f0;                       /* F(m) mod 6 = a(n)              */
}
