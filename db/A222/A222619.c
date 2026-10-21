/*
 * A222619  Number of live cells at the n-th generation of 'spacefiller 1',
 *          utilizing Conway's S23/B3 Game of Life rules.
 * https://oeis.org/A222619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (if n mod 4 == 0 then ((n+19)^2 + 463)/4; if n mod 4 == 2
 *            then ((n+19)^2 + 487)/4; if n is odd then ((n+18)^2 + 639)/4)
 * Range:     u8 none, u16 n <= 493, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result;

    switch (n & 3) {
        case 0: {
            seq_t t = nn + (seq_t)19;
            result = (t * t + (seq_t)463) / (seq_t)4;
            break;
        }
        case 2: {
            seq_t t = nn + (seq_t)19;
            result = (t * t + (seq_t)487) / (seq_t)4;
            break;
        }
        default: /* odd n */
        {
            seq_t t = nn + (seq_t)18;
            result = (t * t + (seq_t)639) / (seq_t)4;
            break;
        }
    }
    return result;
}
