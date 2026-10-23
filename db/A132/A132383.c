/*
 * A132383  4^n written 4^n times.
 * https://oeis.org/A132383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^k where k = floor(log_4(3*n + 1))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 3*n + 1 fits in seq_t for all n whose a(n) fits in seq_t      */
    seq_t x = (seq_t)n * 3 + 1;

    seq_t val = 1;            /* 4^0 */
    while (val <= x / 4) {    /* while val*4 <= x  (no overflow) */
        val *= 4;
    }
    return val;               /* val == 4^k where 4^k <= x < 4^(k+1) */
}
