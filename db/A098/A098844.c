/*
 * A098844  a(1)=1, a(n) = n*a(floor(n/2)).
 * https://oeis.org/A098844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(floor(n/2)), a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 191, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098844:  a(n) = Π_{k≥0} floor(n/2^k)  (product until 0) */
/* Computation is O(log n) using repeated multiplication. */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t   cur  = n;
    while (cur > 0) {
        prod *= cur;
        cur >>= 1;            /* cur = floor(cur / 2) */
    }
    return prod;
}
