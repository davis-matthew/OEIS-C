/*
 * A183648  Number of (n+1) X 6 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 248*4^n + 1806*3^n + 5404*2^n + 8162
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* We assume n >= 1; values for n < 1 are not defined by the sequence. */
    seq_t res = 8162;
    res += 5404 * pow_seq((seq_t)2, n);
    res += 1806 * pow_seq((seq_t)3, n);
    res += 248  * pow_seq((seq_t)4, n);
    res += 5   * pow_seq((seq_t)5, n);
    return res;
}
