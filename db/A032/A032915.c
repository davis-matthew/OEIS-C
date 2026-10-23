/*
 * A032915  Numbers whose set of base-8 digits is {1,3}.
 * https://oeis.org/A032915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032915: numbers whose set of base-8 digits is {1,3} */
seq_t a(idx_t n) {
    idx_t k = 1;
    idx_t cum = 2;                     /* cumulative count up to length k */
    while (cum < n) {
        ++k;
        cum += ((idx_t)1) << k;        /* add 2^k */
    }
    idx_t prev_cum = cum - (((idx_t)1) << k);  /* count up to length k-1 */
    idx_t t = n - prev_cum - 1;                /* offset within block of length k */

    seq_t result = 0;
    for (idx_t j = k; j-- > 0; ) {           /* j from k-1 down to 0 */
        idx_t bit = (t >> j) & 1;
        seq_t digit = 1 + ((seq_t)bit << 1);   /* 1 or 3 */
        result = result * 8 + digit;
    }
    return result;
}
