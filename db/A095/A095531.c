/*
 * A095531  a(n) = 69 written in base 13 - n.
 * https://oeis.org/A095531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal integer whose decimal digits are the
 *            base-(13‑n) representation of the decimal number 69, for n =
 *            0…12 (base 1 gives the unary representation of 69 as 69 ones)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n = 0 … 12.  For larger n the base
       (13 - n) would be ≤ 0 and no representation exists.  We simply return
       0 for such cases as the sequence is undefined beyond n = 12. */
    if (n > 12u)
        return (seq_t)0;

    /* Base for the representation, 13 - n.  For n = 12 this is 1 (unary). */
    idx_t base = 13u - n;

    /* Resulting decimal number (built by concatenating the decimal
       representations of the digits of the base‑b representation of 69). */
    seq_t result = (seq_t)0;

    /* Unary case: base == 1 -> 69 ones. */
    if (base == 1u) {
        for (idx_t i = 0; i < 69u; ++i)
            result = result * (seq_t)10 + (seq_t)1;
        return result;
    }

    /* Compute the digits of 69 in the given base (little‑endian order). */
    idx_t digits[32];   /* 69 takes at most 7 digits in base 2. */
    idx_t len = 0;
    idx_t num = 69u;
    do {
        digits[len++] = num % base;
        num /= base;
    } while (num);

    /* Build the decimal number by appending each digit.  For digits ≥ 10
       we append two decimal digits (e.g., 10 → "10"). */
    for (idx_t i = len; i > 0; --i) {
        idx_t d = digits[i - 1];
        if (d < 10u)
            result = result * (seq_t)10 + (seq_t)d;
        else
            result = result * (seq_t)100 + (seq_t)d;
    }

    return result;
}
