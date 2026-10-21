/*
 * A053645  Distance to largest power of 2 less than or equal to n; write n in
 *          binary, change the first digit to zero, and convert back to
 *          decimal.
 * https://oeis.org/A053645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n - 2^⌊log₂(n)⌋
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the largest power of 2 <= n. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) ++msb;            /* msb = floor(log2(n)) */
    seq_t power = ((seq_t)1) << msb;     /* 2^msb  */
    seq_t nn   = (seq_t)n;              /* cast index to seq_t */
    return nn - power;                  /* difference gives a(n) */
}
