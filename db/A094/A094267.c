/*
 * A094267  First differences of A001511.
 * https://oeis.org/A094267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = v2(n+2) - v2(n+1) where v2(k) is the exponent of 2
 *            dividing k (number of trailing zeros in the binary
 *            representation of k)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute v2 for a 64-bit unsigned integer. */
static inline int v2_idx(idx_t x)
{
    int c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Compute v2(n+1) and v2(n+2).  n is uint64_t; the addition
       is performed in unsigned arithmetic.  Under normal usage the
       result never overflows for the required bounds. */
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;

    int v1 = v2_idx(n1);
    int v2_ = v2_idx(n2);

    int diff = v2_ - v1;
    return (seq_t)diff;
}
