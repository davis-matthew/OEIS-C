/*
 * A095633  a(n) = 121 written in base 11 - n.
 * https://oeis.org/A095633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing the decimal number
 *            121 in base (11 − n) and interpreting the resulting base‑10
 *            string as a decimal number. For n > 10 the sequence is not
 *            defined; this implementation returns 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base in which 121 is represented */
    unsigned long long base = 11ULL - (unsigned long long)n;

    /* Undefined for bases less than 1 */
    if (base == 0)
        return 0;

    /* Base 1 (unary) – the representation is 121 ones */
    if (base == 1)
    {
        seq_t r = 0;
        for (int i = 0; i < 121; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Convert 121 to base 'base', gathering digits (least‑significant first) */
    unsigned int digits[20];
    int count = 0;
    unsigned int value = 121;
    while (value > 0)
    {
        digits[count++] = value % (unsigned int)base;
        value /= (unsigned int)base;
    }

    /* Interpret the digits as a base‑10 number */
    seq_t res = 0;
    for (int i = count - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
