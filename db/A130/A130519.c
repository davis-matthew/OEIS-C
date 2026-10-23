/*
 * A130519  a(n) = Sum_{k=0..n} floor(k/4). (Partial sums of A002265.)
 * https://oeis.org/A130519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} floor(k/4) * Equivalently: a(n) =
 *            floor((n-1)^2/8) * * This implementation uses the recurrence *
 *            a(0) = 0 * a(n) = a(n-1) + floor(n/4) * which is O(n) and works
 *            for arbitrary sized seq_t without risking * overflow on
 *            intermediate products
 * Range:     u8 n <= 46, u16 n <= 725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i;

    /* Sum floor(i/4) for i = 0..n */
    for (i = 0; i <= n; ++i)
        res += i / 4;

    return res;
}
