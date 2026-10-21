/*
 * A064099  a(n) = ceiling(log(3 + 2*n)/log(3)).
 * https://oeis.org/A064099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, easy, nonn
 * Formula:   a(n) = ceil(log(3 + 2*n)/log(3))
 * Range:     u8 n <= 194, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the smallest integer k with 3^k >= 3 + 2*n. */
seq_t a(idx_t n) {
    /* compute the threshold 3 + 2*n */
    seq_t target = (seq_t)n * 2 + 3;
    seq_t pow = 1;     /* 3^0 */
    seq_t k   = 0;     /* answer */
    while (pow < target) {
        pow *= 3;  /* next power of 3 */
        ++k;      /* count the exponent */
    }
    return k;
}
