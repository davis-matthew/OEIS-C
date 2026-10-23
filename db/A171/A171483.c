/*
 * A171483  a(n) = 0+1+2+...+n in lunar arithmetic in base 4 written in base
 *          4.
 * https://oeis.org/A171483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lunar sum of 0..n in base 4, written as a base‑4 digit
 *            string expressed in decimal. For each digit position k, the
 *            digit of a(n) is 3 if n ≥ 3·4^k, otherwise it is floor(n / 4^k)
 *            % 4
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t pow = 1;              /* 4^k */
    unsigned char digits[64];   /* store digits least‑significant first */
    unsigned pos = 0;

    while (1) {
        idx_t nd = n / pow;            /* largest digit in this column */
        seq_t d = nd >= 3 ? 3 : nd;    /* 3 if any number has digit 3 */
        digits[pos++] = (unsigned char)d;

        /* stop when next power of 4 would exceed n */
        if (pow > n / 4) break;
        pow <<= 2;                     /* multiply by 4 */
    }

    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
