/*
 * A140407  A000225 interleaved with A000051.
 * https://oeis.org/A140407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for all n ≥ 0, a(2n) = 2^(n+1) – 1 a(2n+1) = 2^n + 1 (same as
 *            a(2n)=A000225(n+1) and a(2n+1)=A000051(n)+1)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)   /* shift until exp bits set */
        r <<= 1;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {                 /* even index: n = 2k */
        idx_t k = n / 2;
        return power_of_two(k + 1) - 1;    /* 2^(k+1) – 1 */
    } else {                               /* odd index: n = 2k+1 */
        idx_t k = n / 2;
        return power_of_two(k) + 1;        /* 2^k + 1 */
    }
}
