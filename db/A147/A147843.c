/*
 * A147843  a(n) = -n*A010815(n).
 * https://oeis.org/A147843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n * A010815(n) * where A010815(n) is 0 unless n is a
 *            generalized pentagonal number * k(3k±1)/2; in that case
 *            A010815(n) = (-1)^k
 * Range:     i8 n <= 144, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is 0 (formula already gives 0, handled explicitly) */
    if (n == 0)
        return 0;

    /* iterate over k = 1,2,3,… and test both pentagonal forms */
    for (idx_t k = 1; ; ++k)
    {
        /* use 128‑bit temporaries to avoid overflow */
        unsigned __int128 g1 = (unsigned __int128)k * (3 * k - 1) / 2;
        if (g1 == n)
        {
            seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
            return -(seq_t)n * sign;
        }

        unsigned __int128 g2 = (unsigned __int128)k * (3 * k + 1) / 2;
        if (g2 == n)
        {
            seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
            return -(seq_t)n * sign;
        }

        /* stop when both numbers exceed n; future k will only increase them */
        if (g1 > n && g2 > n)
            break;
    }
    return 0;
}
