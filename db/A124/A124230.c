/*
 * A124230  Denominator of g(n) defined by g(1)=1, g(2n)=1/g(n)+1,
 *          g(2n+1)=g(2n).
 * https://oeis.org/A124230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Fibonacci( ceil(log2(n+1)) )
 * Range:     u8 n <= 8191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The Fibonacci sequence is 1-indexed: F(1)=1, F(2)=1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* for robustness, though offset starts at 1 */

    /* Compute k = ceil(log₂(n+1)) = floor_log₂(n) + 1 */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }

    /* Compute Fibonacci(k) iteratively */
    seq_t prev = 1, curr = 1;  /* F(1)=1, F(2)=1 */
    if (k <= 2)
        return 1;

    for (idx_t i = 3; i <= k; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
