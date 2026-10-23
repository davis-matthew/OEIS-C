/*
 * A105529  Given a list of ternary numbers, interpret each as a ternary
 *          modular Gray code number, then convert to decimal.
 * https://oeis.org/A105529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gray base-3 of n, i.e. prefix sums of ternary digits
 *            modulo 3
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* 3^41 > 2^64, so 64 digits are ample for any uint64_t input. */
    const int MAX_DIGITS = 64;
    int digits[MAX_DIGITS];
    int len = 0;
    idx_t tmp = n;

    /* Extract base‑3 digits, least significant first. */
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    }

    seq_t result = 0;
    seq_t prefix = 0;

    /* Process from most significant to least significant. */
    for (int i = len - 1; i >= 0; --i) {
        prefix = (prefix + digits[i]) % 3;
        result = result * 3 + prefix;
    }

    return result;
}
