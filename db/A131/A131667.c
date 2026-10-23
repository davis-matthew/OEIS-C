/*
 * A131667  List of triples 2n, 1-2n, 0, n >= 1.
 * https://oeis.org/A131667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*ceil(n/3) if (n+2)%3==0, * 1-2*ceil(n/3) if (n+2)%3==1,
 *            * 0 otherwise
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131667 – list of triples 2n, 1-2n, 0, n>=1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* offset not used, but safe */
    }
    /* ceil(n/3) = (n+2)/3 for integer n >= 1 */
    seq_t m = ((seq_t)n + 2) / 3;
    idx_t r = ((n % 3) + 2) % 3;   /* (n+2)%3 without overflow */
    if (r == 0) {
        return 2 * m;
    } else if (r == 1) {
        return (seq_t)1 - 2 * m;
    } else {
        return 0;
    }
}
