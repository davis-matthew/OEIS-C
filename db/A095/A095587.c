/*
 * A095587  a(n) = 97 written in base 13 - n.
 * https://oeis.org/A095587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal number whose digits are the representation of 97
 *            * in base (13‑n). For base 1 the representation is 97 ones
 *            (unary)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 0.  For n > 12 (i.e. base ≤ 0) the value is 0. */
seq_t a(idx_t n)
{
    /* Base of the conversion is (13 - n). */
    int base = (int)13 - (int)n;

    /* If base ≤ 0 the sequence is not defined; return 0. */
    if (base <= 0)
        return 0;

    /* Unary (base == 1) case: 97 ones. */
    if (base == 1) {
        seq_t result = 0;
        /* Build the number 111…1 (97 digits).  Works for arbitrary‑precision seq_t. */
        for (int i = 0; i < 97; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* General case: convert 97 to the specified base and then interpret the
       resulting digits as a decimal integer.  For base > 1, the conversion
       yields at most 4 digits (97 < 13^3). */
    seq_t temp   = 97;  /* value to convert */
    seq_t result = 0;   /* decimal value of the digit string */
    seq_t mult   = 1;   /* current place value in base 10 (powers of 10) */

    /* Convert by successive division. */
    while (temp > 0) {
        seq_t digit = temp % (seq_t)base; /* 0 ≤ digit < base ≤ 12 */
        result = result + digit * mult;
        mult   = mult * 10;
        temp   = temp / (seq_t)base;
    }

    return result;
}
