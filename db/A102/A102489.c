/*
 * A102489  Take the decimal representation of n and read it as if it were
 *          written in hexadecimal.
 * https://oeis.org/A102489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   interpret the decimal representation of n as a hexadecimal
 *            number
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for sequence A102489. */
seq_t a(idx_t n)
{
    /* For n = 0 the decimal string is "0". */
    if (n == 0) return 0;

    /* Determine number of decimal digits of n. */
    idx_t tmp = n;
    int len = 0;
    while (tmp > 0) {
        tmp /= 10;
        len++;
    }

    /* Store the digits of n from most‑significant to least‑significant. */
    unsigned char digits[20];          /* max 20 digits for uint64_t */
    tmp = n;
    for (int i = len - 1; i >= 0; --i) {
        digits[i] = tmp % 10;         /* decimal digit */
        tmp /= 10;
    }

    /* Interpret the digits as hexadecimal. */
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = val * ((seq_t)16) + (seq_t)digits[i];
    }
    return val;
}
