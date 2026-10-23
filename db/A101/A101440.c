/*
 * A101440  Replace each digit of n with 1 followed by n 0's: 0 -> 1, 1 -> 10,
 *          2 -> 100, ..., 9 -> 1000000000, 10 -> 101, 11 -> 1010, 12 ->
 *          10100, etc. Expanded number is then converted from binary to
 *          decimal: 0 -> 1 -> 1, 1 -> 10 -> 2, 2 -> 100 -> 4, 9 -> 1000000000
 *          -> 512, 10 -> 101 -> 5, etc.
 * https://oeis.org/A101440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = interpreted binary value of the concatenation of the
 *            strings “1” followed by d zeroes for each decimal digit d of n
 *            (e.g., n = 123 → “10 100 1000” → 328)
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: decimal digit 0 gives the binary string "1" */
    if (n == 0)
        return (seq_t)1;

    /* Reverse digits of n (max 20 decimal digits for uint64_t) */
    int rev[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    /* Process digits from most significant to least significant */
    for (int i = len - 1; i >= 0; --i) {
        int d = rev[i];                    /* decimal digit */
        seq_t pattern = (seq_t)((uint64_t)1 << d);  /* 1 followed by d zeros */
        if (result == 0) {                 /* first digit: no shift needed */
            result = pattern;
        } else {
            result <<= (d + 1);            /* shift left by the length of this part */
            result |= pattern;            /* append the new part */
        }
    }
    return result;
}
