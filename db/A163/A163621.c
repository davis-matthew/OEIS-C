/*
 * A163621  Square array read by antidiagonals: Write n and m in binary with n
 *          on the left. Concatenate. a(n,m) is the decimal equivalent of the
 *          result.
 * https://oeis.org/A163621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) is the n-th term of A163621. The sequence is obtained by
 *            traversing the infinite array {a(i,j)} (i,j ≥ 1) in antidiagonal
 *            order. For each element the value is the integer whose binary
 *            representation is the concatenation of the binary for i followed
 *            by the binary for j. a(n) returns the decim...
 * Range:     u8 n <= 260, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t diag_of(idx_t n)
{
    /* Find the smallest d such that d*(d+1)/2 >= n.  d is the
       number of diagonals processed up to and including the
       diagonal containing the n-th element. */
    if (n <= 1)
        return 1;
    idx_t low  = 1;
    idx_t high = 2;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the diagonal and position within it. */
    idx_t d   = diag_of(n);                                 /* diagonal count */
    idx_t prev = ((idx_t)(__int128)(d - 1) * d / 2);        /* elements before diagonal d */
    idx_t pos  = n - prev - 1;                              /* 0‑based offset in diagonal d */

    /* For diagonal d the sum of indices is d+1.
       The pairs appear in order of decreasing i and increasing j. */
    idx_t i = d - pos;          /* row index  (i ≥ 1) */
    idx_t j = pos + 1;          /* column index (j ≥ 1) */

    /* Compute bit length of j (number of bits needed to represent j). */
    unsigned bitlen = 64 - __builtin_clzll((unsigned long long)j);

    /* Concatenate the binary of i and j: shift i left and add j. */
    seq_t value = ((seq_t)i << bitlen) | (seq_t)j;
    return value;
}
