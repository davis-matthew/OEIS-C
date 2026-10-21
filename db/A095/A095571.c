/*
 * A095571  a(n) = 89 written in base 12 - n.
 * https://oeis.org/A095571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the digits of the decimal number 89 written in base
 *            (12-n), interpreted as a decimal integer. For n=0..10 this
 *            yields the usual base‑representation; for n=11 the base is 1
 *            (unary), producing 89 ones
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The definition is only valid for 0 ≤ n ≤ 11.  For larger n we return 0. */
    if (n > 11)
        return 0;

    /* Base for representation: 12 - n.  For n = 11 this equals 1 (unary). */
    unsigned int base = 12 - (unsigned int)n;

    seq_t result = 0;

    if (base == 1) {
        /* Unary representation: 89 ones.  Build the number digit by digit. */
        for (idx_t i = 0; i < 89; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Convert decimal 89 into base `base`.  The remainder will always be <10
       for the supplied bases (2–12).  Store the digits in reverse order
       (least significant first). */
    unsigned int temp    = 89;
    unsigned char digits[10];   /* 89 < 2^7, so at most 7 digits in binary. */
    int dcnt = 0;

    while (temp > 0) {
        digits[dcnt++] = temp % base;
        temp /= base;
    }

    /* Build the decimal number by treating the digits as decimal digits. */
    for (int i = dcnt - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
