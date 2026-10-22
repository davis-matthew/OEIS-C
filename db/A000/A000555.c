/*
 * A000555  Number of labeled trees of diameter 4 with n nodes.
 * https://oeis.org/A000555
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=2..n-3} binomial(n-1,k) * (k^(n-1-k) - k) (a
 *            center vertex, k neighbors of it, and the other n-1-k vertices
 *            hung on those neighbors, with at least two neighbors used);
 *            equals A000551(n)-n*(n-1)*(2^(n-2)-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 28,
 *            big any n
 * Helpers:   g555, p555
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g555.h"
#include "../utils/p555.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    idx_t m = n - 1;
    seq_t C = (seq_t)m * ((seq_t)m - 1) / 2; /* binomial(m,2) */
    seq_t s = 0;
    idx_t k;
    for (k = 2; k + 2 <= m; k++) {
        s += C * (p555((seq_t)k, m - k) - (seq_t)k);
        /* C(m,k+1) = C(m,k) * (m-k)/(k+1), done without growing intermediates */
        idx_t num = m - k, den = k + 1;
        idx_t g = g555(num, den);
        num /= g; den /= g;
        C = C / (seq_t)den * (seq_t)num;
    }
    return (seq_t)n * s;
}
