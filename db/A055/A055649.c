/*
 * A055649  Integers in base 11 with each base-11 digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret n expressed in base 11 as a number in base 100,
 *            i.e. write each base‑11 digit as a two‑digit decimal string,
 *            concatenate them, and read the result as a decimal integer
 * Range:     u8 n <= 32, u16 n <= 846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* store base‑11 digits, least significant first */
    idx_t digits[64];
    idx_t sz = 0;

    if (n == 0) {
        return 0;
    }

    while (n != 0) {
        digits[sz++] = n % 11;
        n /= 11;
    }

    seq_t result = 0;
    for (idx_t i = sz; i > 0; ) {
        --i;
        result = result * 100 + digits[i];
    }

    return result;
}
