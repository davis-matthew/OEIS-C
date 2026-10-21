/*
 * A095524  a(n) = 66 written in base n.
 * https://oeis.org/A095524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 66 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special‐case base 1 (unary).  66 ones. */
    if (n == 1)
    {
        seq_t r = 0;
        for (idx_t i = 0; i < 66; ++i)
            r = r * 10 + 1;      /* 111…111 (66 digits) */
        return r;
    }

    /* 0 is not a valid base here; return 0 for safety. */
    if (n <= 1)
        return 0;

    /* Compute the digits of 66 in base n. */
    /* The maximum number of digits needed is 7 (for base 2). */
    uint8_t d[16];
    idx_t len = 0;
    idx_t temp = 66;
    while (temp > 0)
    {
        d[len++] = (uint8_t)(temp % n);
        temp /= n;
    }
    if (len == 0)          /* n = 0, should not happen, return 0. */
        return 0;

    /* Build the decimal integer by concatenating the base‑n digits. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i)
    {
        uint8_t digit = d[len - 1 - i];   /* most significant first */
        if (digit < 10)
            result = result * 10 + digit;
        else
            result = result * 100 + digit;  /* digits 10‑99 give two decimal digits */
    }
    return result;
}
