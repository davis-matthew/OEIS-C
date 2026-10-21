/*
 * A218442  a(n) = Sum_{k=0..n} floor(n/(3*k + 1)).
 * https://oeis.org/A218442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(n/(3*k + 1))
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the sum is empty; return 0 immediately. */
    if (n == 0) return 0;

    seq_t sum = 0;
    /* Terms with 3*k+1 > n contribute 0, so we only iterate while 3*k+1 <= n. */
    idx_t kmax = (n - 1) / 3;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t d = 3 * k + 1;  /* d <= n for all iterations */
        sum += (seq_t)(n / d);
    }
    return sum;
}
