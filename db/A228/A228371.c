/*
 * A228371  First differences of A228370. Also A001511 and A006519
 *          interleaved.
 * https://oeis.org/A228371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(2n-1) = v2(n) + 1 , a(2n) = 2^v2(n)
 * Range:     u8 n <= 128, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {                     /* even index */
        idx_t k = n >> 1;
        unsigned int tz = 0;
        while ((k & 1ULL) == 0) {
            k >>= 1;
            tz++;
        }
        return (seq_t)(1ULL << tz);            /* 2^v2(k) */
    } else {                                   /* odd index */
        idx_t k = (n + 1) >> 1;
        unsigned int tz = 0;
        while ((k & 1ULL) == 0) {
            k >>= 1;
            tz++;
        }
        return (seq_t)(tz + 1);                /* v2(k) + 1 */
    }
}
