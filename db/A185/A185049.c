/*
 * A185049  Last term in the continued fraction for
 *          floor(Fibonacci(n)*(1+sqrt(5))/2) / Fibonacci(n).
 * https://oeis.org/A185049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=1; a(3)=2; a(4)=3; a(5)=2; a(6)=2; For n>6, the
 *            sequence repeats every 4 with pattern [2,3,2,2]; equivalently,
 *            a(n)=a(n-4) for n>6
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly. */
    if (n <= 6)
    {
        static const seq_t first6[6] = {1, 1, 2, 3, 2, 2};
        return first6[n - 1];
    }

    /* For n > 6 the pattern [2,3,2,2] repeats. */
    static const seq_t pat[4] = {2, 3, 2, 2};
    return pat[(n - 7) % 4];
}
