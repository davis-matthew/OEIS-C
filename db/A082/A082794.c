/*
 * A082794  Smallest multiple of n beginning with 4.
 * https://oeis.org/A082794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest multiple of n whose decimal representation
 *            starts with the digit 4 (search algorithm)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t first_digit(seq_t x)
{
    /* Return the leading decimal digit of x.  x is assumed to be
       a non‑negative integer of type seq_t. */
    while (x >= 10)
        x /= 10;
    return x;
}

seq_t a(idx_t n)
{
    seq_t mul = (seq_t)n;   /* constant factor n  */
    seq_t cur = 0;          /* current product n*k   */

    for (;;)                  /* iterate k = 1,2,3,…  */
    {
        cur += mul;           /* cur = n * k           */
        if (first_digit(cur) == 4)
            return cur;       /* found the smallest multiple starting with 4 */
    }
}
