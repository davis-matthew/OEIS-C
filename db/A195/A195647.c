/*
 * A195647  a(n) is the optimal wire-length for an n X n grid.
 * https://oeis.org/A195647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n^2 + n - 2) - t*(2t^2 - 6nt + 3n^2 + 3n - 2)/3, where
 *            t = round((6n - sqrt(6*(3n^2 - 3n + 2)))/6)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: All calculations are performed with integer arithmetic only. */

/* Integer square root: floor(sqrt(x)) for any seq_t */
static seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        if (mid == 0) {
            low = 1;
            continue;
        }
        seq_t div = x / mid;            /* avoid overflow */
        if (div >= mid)                 /* mid*mid <= x  */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* use seq_t for all arithmetic */
    seq_t nn   = (seq_t)n;        /* n */
    seq_t nnn  = nn * nn;          /* n^2 */

    /* compute t */
    seq_t valS = 6 * (3 * nnn - 3 * nn + 2);
    seq_t sqrtS = floor_sqrt(valS);
    seq_t t_val = (6 * nn - sqrtS + 2) / 6;

    /* first term: n*(n^2 + n - 2) */
    seq_t first = nn * (nnn + nn - 2);

    /* second term: t*(2t^2 - 6nt + 3n^2 + 3n - 2)/3 */
    seq_t two_t2 = 2 * t_val * t_val;
    seq_t six_nt = 6 * nn * t_val;
    seq_t three_n2 = 3 * nnn;
    seq_t subexpr = two_t2 - six_nt + three_n2 + 3 * nn - 2;
    seq_t second = t_val * subexpr / 3;

    return first - second;
}
