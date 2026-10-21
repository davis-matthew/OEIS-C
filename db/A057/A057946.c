/*
 * A057946  Bad hexadecimal notation for n: write n in hexadecimal notation,
 *          replacing digit ten with "10", digit eleven with "11", ... and
 *          digit fifteen with "15".
 * https://oeis.org/A057946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, dumb, easy, nonn
 * Formula:   a(n) is obtained by writing n in hexadecimal and replacing each
 *            digit 10–15 by the two‑character decimal string "10"–"15". The
 *            result is the decimal value of the resulting string
 * Range:     u8 n <= 57, u16 n <= 1961, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* maximum number of hex digits for a 64‑bit integer is 16 */
    idx_t digits[16];
    idx_t pos = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[pos++] = temp & 0xF;   /* temp % 16 */
        temp >>= 4;                  /* temp /= 16 */
    }

    seq_t res = 0;
    for (idx_t i = pos; i-- > 0; ) {
        idx_t d = digits[i];
        if (d < 10) {
            res = res * 10 + d;
        } else {
            res = res * 10 + 1;
            res = res * 10 + (d - 10);
        }
    }
    return res;
}
