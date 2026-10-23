/*
 * A141782  Number of connected graphs with one cycle of length m = n-4 and n
 *          nodes.
 * https://oeis.org/A141782
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 9n^2 - n + 87)/48 for odd n; a(n) = (n^3 + 9n^2 +
 *            8n + 192 - (n mod 4)*6)/48 for even n
 * Range:     u8 none, u16 n <= 37, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;  /* offset is 7; values below are irrelevant */

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t num;

    if (n & 1) {  /* n is odd */
        num = n3 + 9 * n2 - nn + 87;
    } else {      /* n is even */
        num = n3 + 9 * n2 + 8 * nn + 192 - ((n & 3) * 6);
    }

    return num / 48;
}
