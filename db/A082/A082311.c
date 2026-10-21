/*
 * A082311  A Jacobsthal sequence trisection.
 * https://oeis.org/A082311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*8^n + (-1)^n)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation of 8^n */
    seq_t result_pow = (seq_t)1;
    seq_t base = (seq_t)8;
    idx_t exp = n;
    while (exp != 0) {
        if (exp & 1)
            result_pow = result_pow * base;
        base = base * base;
        exp >>= 1;
    }

    /* compute (2*pow) +/- 1 depending on parity of n, then divide by 3 */
    if (n & 1) {                /* odd n: (-1)^n = -1 */
        return ((result_pow * (seq_t)2) - (seq_t)1) / (seq_t)3;
    } else {                    /* even n: (-1)^n = 1 */
        return ((result_pow * (seq_t)2) + (seq_t)1) / (seq_t)3;
    }
}
