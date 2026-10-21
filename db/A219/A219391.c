/*
 * A219391  Numbers k such that 21*k + 1 is a square.
 * https://oeis.org/A219391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((21*⌊(n-1)/4⌋ + r)^2 - 1) / 21, where r = 1, 8, 13, 20
 *            correspond to (n-1) mod 4 = 0, 1, 2, 3 respectively.
 *            Equivalently: let m = ⌊(n-1)/4⌋. if (n-1)%4 == 0 → a = m*(21*m +
 *            2) if (n-1)%4 == 1 → a = m*(21*m + 16) + 3 if (n-1)%4 == 2 → a =
 *            m*(21*m + 26) + 8 if (n-1)%4 == 3 → a = m*(21*m + 4...
 * Range:     u8 n <= 14, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q   = (n - 1) / 4;
    int   r   = (int)((n - 1) % 4);
    seq_t m   = (seq_t)q;
    seq_t res;

    switch (r) {
    case 0: /* r = 1 */
        res = m * (((seq_t)21 * m) + (seq_t)2);
        break;
    case 1: /* r = 8 */
        res = m * (((seq_t)21 * m) + (seq_t)16) + (seq_t)3;
        break;
    case 2: /* r = 13 */
        res = m * (((seq_t)21 * m) + (seq_t)26) + (seq_t)8;
        break;
    case 3: /* r = 20 */
        res = m * (((seq_t)21 * m) + (seq_t)40) + (seq_t)19;
        break;
    default:
        res = 0; /* should never happen */
    }

    return res;
}
