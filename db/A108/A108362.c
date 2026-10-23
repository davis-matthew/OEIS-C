/*
 * A108362  Pair reversal of Fibonacci numbers.
 * https://oeis.org/A108362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=2, a(3)=1, a(n)=3*a(n-2)-a(n-4) */
seq_t a(idx_t n)
{
        if (n == 0) return 1;
        if (n == 1) return 0;
        if (n == 2) return 2;
        if (n == 3) return 1;
        seq_t a0 = 1, a1 = 0, a2 = 2, a3 = 1, an;
        for (idx_t i = 4; i <= n; ++i) {
                an = 3 * a2 - a0;
                a0 = a1;
                a1 = a2;
                a2 = a3;
                a3 = an;
        }
        return an;
}
