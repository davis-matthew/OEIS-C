/*
 * A060585  Write n in base 3, then (working from left to right) if the k-th
 *          digit of n is not equal to the digit to its left then the k-th
 *          digit of a(n) is 1, otherwise it is 0, and finally read the result
 *          as a base-2 number.
 * https://oeis.org/A060585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = read the ternary representation of n; produce a binary
 *            number whose bits indicate digit changes (the first digit is
 *            always 1); a(0)=0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* store ternary digits, least‑significant first */
    int d[64];
    int pos = 0;
    idx_t m = n;
    while (m > 0) {
        d[pos++] = m % 3;
        m /= 3;
    }

    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i) {
        seq_t bit = (i == pos - 1) ? 1
                                   : (d[i] != d[i + 1]) ? 1 : 0;
        res = (res << 1) | bit;
    }
    return res;
}
