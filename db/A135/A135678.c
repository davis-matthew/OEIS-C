/*
 * A135678  Floor(n^(4/3)+n).
 * https://oeis.org/A135678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(4/3)+n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cube_root_floor(seq_t x)
{
    /* binary search for largest t such that t^3 <= x */
    seq_t low  = 0;
    seq_t high = 1;
    /* increase high until high^3 > x */
    while (1) {
        seq_t cube = high * high * high;
        if (cube > x) break;
        high = high * 2;
    }
    /* binary search between low and high */
    while (high - low > 1) {
        seq_t mid = (low + high) >> 1;
        if (mid * mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* compute n^4 as seq_t to avoid overflow of idx_t */
    seq_t tn = (seq_t)n;
    seq_t n4 = tn * tn * tn * tn;          /* n^4 */
    seq_t cbr = cube_root_floor(n4);       /* floor( (n^4)^(1/3) ) == floor(n^(4/3)) */
    return tn + cbr;                       /* floor(n + n^(4/3)) */
}
