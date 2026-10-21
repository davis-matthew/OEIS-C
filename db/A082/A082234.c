/*
 * A082234  In the following square array numbers are entered like this
 *          A(1,1), A(1,2), A(2,1), A(3,1), A(2,2), A(1,3), A(1,4), A(2,3),
 *          A(3,2), A(4,1), A(5,1), A(4,2), ... such that every entry is the
 *          geometric mean of the two diametrically opposite neighbors
 *          (wherever a pair exists).
 * https://oeis.org/A082234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(r-1)*2^(s-1) where (r,s) are the coordinates of the
 *            n‑th term in the OEIS anti‑diagonal ordering of the array A(r,s)
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* below the offset – behaviour irrelevant */
        return (seq_t)0;

    /* Find the diagonal d (sum of indices r+s) that contains the n‑th term. */
    idx_t d   = 2;            /* first diagonal has d = 2 (1,1) */
    idx_t sum = 1;            /* cumulative count of terms up to diagonal d */

    while (sum < n) {
        ++d;
        sum += d - 1;        /* diagonal d has length d‑1 */
    }

    /* Offset of the term inside this diagonal (1‑based). */
    idx_t prev   = sum - (d - 1);   /* count of terms before this diagonal */
    idx_t offset = n - prev;        /* 1‑based position within the diagonal */

    /* Coordinates (r,s) depend on the parity of d. */
    idx_t r, s;
    if ((d & 1) == 0) {           /* even diagonal: reversed order */
        r = d - offset;           /* (d-1,1) … (1,d-1) */
        s = offset;
    } else {                       /* odd diagonal: normal order */
        r = offset;               /* (1,d-1) … (d-1,1) */
        s = d - offset;
    }

    /* Compute 3^(r-1) and 2^(s-1) using only integer arithmetic. */
    seq_t prod = 1;
    for (idx_t i = 0; i < r - 1; ++i)
        prod *= (seq_t)3;

    seq_t pow2 = 1;
    for (idx_t i = 0; i < s - 1; ++i)
        pow2 *= (seq_t)2;

    return prod * pow2;
}
