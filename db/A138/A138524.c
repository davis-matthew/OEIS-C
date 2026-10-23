/*
 * A138524  a(n) = Sum_{k=1..n} (2*k)!.
 * https://oeis.org/A138524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (2*k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  factorial_even holds (2*(k-1))! on each loop iteration.
        We update it to (2*k)! by multiplying by (2*k-1) and (2*k),
        then add it to the cumulative sum. */
    seq_t factorial_even = 1; /* (2*0)! = 0! = 1 */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        factorial_even *= (seq_t)(2 * k - 1);
        factorial_even *= (seq_t)(2 * k);
        sum += factorial_even;
    }

    return sum;
}
