/*
 * A101610  n! * Sum[k=1..ceiling(n/2), 1/k].
 * https://oeis.org/A101610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum[k=1..ceil(n/2), 1/k]
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, return 0 for safety */

    /* ceil(n/2) for unsigned n */
    idx_t m = (n + 1) / 2;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* sum n!/k for k=1..m */
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k)
        sum += fact / (seq_t)k;

    return sum;
}
