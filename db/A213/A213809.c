/*
 * A213809  Position of the maximum element in the simple continued fraction
 *          of Fibonacci(n+1)^5/Fibonacci(n)^5.
 * https://oeis.org/A213809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1..4; otherwise let k = n/10, m = n - 10*k. If m
 *            == 2: a(n) = 1 + 2*k; else a(n) = 3 + 2*k
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return 1;

    idx_t k = n / 10;
    idx_t m = n - 10 * k;

    if (m == 2)
        return (seq_t)(1 + 2 * k);
    else
        return (seq_t)(3 + 2 * k);
}
