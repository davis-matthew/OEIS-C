/*
 * A222241  In the number n, replace all (decimal) digits '4' with '6' and
 *          vice versa.
 * https://oeis.org/A222241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n with digits 4 and 6 swapped
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digit swap table: 0→0, 1→1, 2→2, 3→3, 4→6, 5→5, 6→4, 7→7, 8→8, 9→9 */
    static const unsigned char swap[10] = {
        0, 1, 2, 3, 6, 5, 4, 7, 8, 9
    };

    /* maximum decimal digits for a 64‑bit integer */
    unsigned char digits[20];
    int len = 0;

    if (n == 0) {
        /* explicit handling of 0 */
        digits[0] = 0;
        len = 1;
    } else {
        seq_t temp = (seq_t)n;
        while (temp > 0) {
            digits[len++] = (unsigned char)(temp % 10);
            temp /= 10;
        }
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + (seq_t)swap[digits[i]];
    }

    return res;
}
