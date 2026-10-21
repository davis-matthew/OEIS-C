/*
 * A072283  Number of raised dots to represent n in Braille.
 * https://oeis.org/A072283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4 + sum of Braille dot counts for each decimal digit of n
 *            (digit → dots: 0→3, 1→1, 2→2, 3→2, 4→3, 5→2, 6→3, 7→4, 8→3, 9→2)
 * Range:     u8 n <= 9340, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char braille_dots[10] = {3,1,2,2,3,2,3,4,3,2};

seq_t a(idx_t n)
{
    seq_t total = 4;                      /* numeral sign */
    if (n == 0) {
        total += braille_dots[0];
    } else {
        idx_t x = n;
        while (x != 0) {
            int d = x % 10;                /* current decimal digit */
            total += braille_dots[d];
            x /= 10;
        }
    }
    return total;
}
