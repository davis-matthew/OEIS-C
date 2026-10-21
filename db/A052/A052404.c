/*
 * A052404  Numbers without 2 as a digit.
 * https://oeis.org/A052404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d > 1 by d + 1 in base-9 representation of
 *            n-1
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the nth non‑negative integer (with offset 1) that contains no
 * decimal digit '2'. */
seq_t a(idx_t n)
{
    idx_t k = n - 1;          /* index starting from 0 */
    seq_t result = 0;         /* result accumulates decimal digits */
    seq_t pow10  = 1;         /* current 10^pos in decimal */

    while (k > 0) {
        idx_t d = k % 9;      /* least significant base‑9 digit */
        k   = k / 9;          

        seq_t mapped = d;      /* digit to place in decimal */
        if (d > 1) mapped = mapped + 1;  /* shift digits >1 up by 1 */

        result += mapped * pow10;  /* place mapped digit */
        pow10  *= 10;              /* next decimal position */
    }

    return result;
}
