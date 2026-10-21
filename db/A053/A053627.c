/*
 * A053627  Smallest integer which is the harmonic mean of n and an integer.
 * https://oeis.org/A053627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest integer that is the harmonic mean of n and some integer.
   It equals 2*n minus the largest divisor of 2*n*n that is strictly less than 2*n. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* not used, but safe for n=0 */
    seq_t two_n2 = (seq_t)2 * (seq_t)n * (seq_t)n; /* 2·n², fits in seq_t for n≤~10⁴ */
    idx_t limit = 2 * n;                  /* 2·n, used as upper bound for a divisor */

    /* Search for the largest divisor d of two_n2 with d < 2·n. */
    for (idx_t d = limit - 1; d > 0; --d) {
        if (two_n2 % (seq_t)d == 0) {
            return (seq_t)(limit - d);   /* a(n) = 2·n – d */
        }
    }
    return (seq_t)1;                     /* unreachable for n≥1 */
}
