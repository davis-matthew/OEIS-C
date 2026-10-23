/*
 * A027625  Numerator of n*(n+5)/((n+2)*(n+3)).
 * https://oeis.org/A027625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of n*(n+5)/((n+2)*(n+3))
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k;
    unsigned __int128 prod;
    seq_t res;

    switch (n % 3) {
        case 0: /* n = 3k : a(n) = ½·k·(3k+5) */
            k = n / 3;
            {
                unsigned __int128 k128 = k;
                prod = k128 * (3 * k128 + 5);
            }
            break;

        case 1: /* n = 3k+1 : a(n) = ½·(k+2)·(3k+1) */
            k = (n - 1) / 3;
            {
                unsigned __int128 k128 = k;
                prod = (k128 + 2) * (3 * k128 + 1);
            }
            break;

        case 2: /* n = 3k+2 : a(n) = ½·(3k+2)·(3k+7) */
            k = (n - 2) / 3;
            {
                unsigned __int128 k128 = k;
                prod = (3 * k128 + 2) * (3 * k128 + 7);
            }
            break;
    }

    res = (seq_t)(prod / 2);
    return res;
}
