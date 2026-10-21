/*
 * A064054  Tenth column of trinomial coefficients.
 * https://oeis.org/A064054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*(n^4 + 66 n^3 + 1307 n^2 + 8706 n +
 *            15120) / 3024
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 40, u64 n <= 564, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an 128‑bit intermediate to avoid overflow when computing binomials and the final sum. */
#include <stdint.h>

static seq_t binom_n_k(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n+1-i) / i  using 128‑bit arithmetic */
        unsigned __int128 tmp = (unsigned __int128)res * (n + 1 - i);
        tmp /= i;
        res = (seq_t)tmp;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = n + 5;
    unsigned __int128 total = 0;

    total += (unsigned __int128)binom_n_k(m, 5) * 5;      /* 5 * C(m,5)   */
    total += (unsigned __int128)binom_n_k(m, 6) * 20;     /* 20*C(m,6)    */
    total += (unsigned __int128)binom_n_k(m, 7) * 21;     /* 21*C(m,7)    */
    total += (unsigned __int128)binom_n_k(m, 8) * 8;      /* 8 * C(m,8)   */
    total += (unsigned __int128)binom_n_k(m, 9);          /* C(m,9)       */

    return (seq_t)total;
}
