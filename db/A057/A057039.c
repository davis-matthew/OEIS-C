/*
 * A057039  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057039(n)=j(2n).
 * https://oeis.org/A057039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = j(2n)
 *  where (i,j) is the pair at position 2n in the diagonal
 *  enumeration 1; 2,3; 4,5,6; …   (i increases along each diagonal).
 *  The algorithm finds the diagonal d containing 2n, then returns the
 *  column index j = d - (2n - d*(d-1)/2 - 1).  All arithmetic is done
 *  with idx_t and seq_t, avoiding floating point. */
seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0) return (seq_t)0;           /* safety for n=0 */

    /* k = 2n is the index of the element whose second coordinate we need */
    idx_t k = n << 1;                      /* 2*n */

    /* Find the minimal d such that d*(d+1)/2 >= k */
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until it satisfies the inequality */
    for (;;) {
        seq_t prod = (seq_t)high * (high + 1) / 2;
        if (prod >= k) break;
        high <<= 1;                          /* high *= 2 */
    }
    /* Binary search between low+1 and high to find minimal d */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        seq_t prod = (seq_t)mid * (mid + 1) / 2;
        if (prod >= k) high = mid;
        else           low  = mid;
    }
    idx_t d = high;                            /* diagonal number */

    /* Compute the offset of k within this diagonal */
    seq_t prefix = (seq_t)d * (d - 1) / 2;      /* elements before diagonal d */
    seq_t offset = (seq_t)k - prefix - 1;       /* 0‑based index on diagonal d */

    /* The second coordinate j is d - offset */
    seq_t j = (seq_t)d - offset;
    return j;
}
