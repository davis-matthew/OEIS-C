/*
 * A212494  Base 2i representation of nonnegative integers.
 * https://oeis.org/A212494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal number whose digits are the base‑2i
 *            representation (digits 0–3) of the non‑negative integer n. For a
 *            real integer n the odd‑position digits are zero, so the base‑2i
 *            digits are obtained by first writing n in base –4 (digits 0–3)
 *            and then inserting a zero after each digit
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 819, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* a(0) = 0 */

    /* Convert n to base –4.  Digits are stored least‑significant first. */
    unsigned char d[128];          /* enough for n up to 2^64‑1 */
    int len = 0;
    __int128 x = n;                /* signed 128‑bit for division by –4 */

    while (x != 0) {
        int rem = (int)(x % -4);   /* remainder may be negative          */
        x /= -4;                   /* integer division towards zero      */
        if (rem < 0) {             /* make rem 0..3, adjust quotient      */
            rem += 4;
            x += 1;
        }
        d[len++] = (unsigned char)rem;
    }

    /* Build the decimal integer described by the digits with zeros
       interleaved between consecutive positions in base 2i. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + d[i];
        if (i > 0)                  /* insert zero for odd powers of 2i */
            res = res * 10;
    }
    return res;
}
