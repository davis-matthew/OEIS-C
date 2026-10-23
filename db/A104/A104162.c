/*
 * A104162  Indicator sequence for the Fibonacci numbers.
 * https://oeis.org/A104162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is a Fibonacci number (>1), a(1) = 2, a(0) = 1,
 *            otherwise a(n) = 0. Computed by generating Fibonacci numbers up
 *            to n. (a(0)=1, a(1)=2, a(10)=0, a(13)=1, …)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    idx_t f0 = 1, f1 = 1;
    while (f1 < n) {
        idx_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    return (f1 == n) ? (seq_t)1 : (seq_t)0;
}
