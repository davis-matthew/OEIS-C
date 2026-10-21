/*
 * A057703  a(n) = n*(94 + 5*n + 25*n^2 - 5*n^3 + n^4)/120.
 * https://oeis.org/A057703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..5} binomial(n, j)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 220, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;
    /* Factorials for 0! .. 5!  */
    static const unsigned int fact[6] = {1, 1, 2, 6, 24, 120};

    for (idx_t k = 1; k <= 5 && k <= n; ++k) {
        unsigned __int128 prod = 1;
        for (idx_t i = 0; i < k; ++i) {
            prod *= (unsigned __int128)(n - i);
        }
        prod /= fact[k];
        result += (seq_t)prod;
    }

    return result;
}
