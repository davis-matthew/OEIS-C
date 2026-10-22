/*
 * A001290  Erroneous version of A028368.
 * https://oeis.org/A001290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^((n+1)*(n+2)/2) * Product_{k=2..n} (2^k - 1) (gives
 *            192, 21504, 10321920, ...); the dead OEIS entry lists a(4) as
 *            10321924, a typo it keeps as data, so that one value is
 *            reproduced
 * Range:     i8 none, i16 n <= 3, i32 n <= 4, i64 n <= 4, i128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t p = 2;
    idx_t k;
    idx_t e;
    if (n == 4)
        return (seq_t)10321924;
    for (k = 2; k <= n; k++) {
        p = p * 2;
        r = r * (p - 1);
    }
    e = (n + 1) * (n + 2) / 2;
    r = r << e;
    return r;
}
