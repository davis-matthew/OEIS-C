/*
 * A001736  4 in base 10-n.
 * https://oeis.org/A001736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = the number 4 written in base 10-n, with its digits read
 *            as a decimal number (for base 1, 4 is written in unary as four
 *            1's)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long x = 4;
    unsigned long long b, k;
    seq_t result = 0, power = 1;
    unsigned long long num = x;

    if (n > 9)
        return 0;
    b = 10 - (unsigned long long)n;

    if (b == 1) {
        for (k = 0; k < x; k++)
            result = result * 10 + 1;
        return result;
    }

    while (num > 0) {
        result += (seq_t)(num % b) * power;
        power *= 10;
        num /= b;
    }
    return result;
}
