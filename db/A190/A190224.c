/*
 * A190224  a(n) = [n*u + n*v] - [n*u] - [n*v], where u=sin(Pi/3),
 *          v=cos(Pi/3), and []=floor.
 * https://oeis.org/A190224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*u + n*v] - [n*u] - [n*v] where u = sin(Pi/3), v =
 *            cos(Pi/3), and [] = floor
 * Range:     u8 n <= 10, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root: floor(sqrt(v)) for any seq_t value v */
static seq_t seq_floor_sqrt(seq_t v)
{
    if (v == 0) return 0;
    seq_t high = 1;
    /* increase high until high > sqrt(v) */
    while (high <= v / high) {
        high += high;               /* double the high value */
    }
    seq_t low = high / 2;
    /* binary search between low and high */
    while (low + 1 < high) {
        seq_t mid = (low + high) / 2;
        if (mid <= v / mid) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* even indices are always zero */
    if ((n & 1ULL) == 0) {
        return 0;
    }

    /* tmp = 3 * n * n, cast to seq_t to preserve precision */
    seq_t tmp = (seq_t)n;
    tmp *= tmp;       /* n * n */
    tmp *= (seq_t)3;  /* 3 * n^2 */

    /* t = floor(n * sqrt(3)) */
    seq_t t = seq_floor_sqrt(tmp);

    /* a(n) = 1 iff t is odd (n is odd) */
    return t & 1;
}
