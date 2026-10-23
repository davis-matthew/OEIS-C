/*
 * A175263  Positive integers that in binary contain an odd number of digits
 *          with the middle digit being a 0.
 * https://oeis.org/A175263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is the n-th positive integer whose binary representation
 *            contains an odd number of bits and has the middle bit equal to 0
 * Range:     u8 n <= 42, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the block (odd length = 2*k+1) that contains the n-th term. */
    idx_t cumulative = 0;
    idx_t k = 0;                     /* k = 0 corresponds to length 1 (invalid), start from 1 */
    while (1) {
        k++;                          /* Now k >= 1, length = 2*k+1 */
        /* Count of numbers in this block: 2^(2*k-1) for k>=1 */
        idx_t block = ((idx_t)1 << (2 * k - 1));
        if (cumulative + block >= n) break;
        cumulative += block;
    }

    /* Position within the block (1-based). */
    idx_t offset = n - cumulative;   /* 1 <= offset <= count of this block */

    /* Number of high and low bits in this block. */
    idx_t high_count = ((idx_t)1 << (k - 1));    /* k-1 bits above the middle */
    idx_t low_count  = ((idx_t)1 << k);          /* k bits below the middle */

    /* Zero-based index inside the block. */
    idx_t idx = offset - 1;

    /* Split into high and low parts. */
    idx_t high = idx / low_count;   /* 0 .. high_count-1 */
    idx_t low  = idx % low_count;   /* 0 .. low_count-1 */

    /* Construct the number. */
    seq_t num = 0;
    num |= ((seq_t)1 << (2 * k));          /* most significant bit */
    num |= ((seq_t)high << (k + 1));       /* high bits */
    num |= ((seq_t)low);                  /* low bits */

    return num;
}
