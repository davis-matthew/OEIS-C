/*
 * A223133  Number of distinct sums i+j+k with i,j,k >= 0, i*j*k <= n and
 *          gcd(i,j,k) <= 1.
 * https://oeis.org/A223133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct sums i+j+k with 0 ≤ i,j,k ≤ n,
 *            i·j·k ≤ n and gcd(i,j,k) ≤ 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Euclidean gcd for idx_t values (idx_t is uint64_t). */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    const idx_t maxSum = 3 * n;
    bool *has = (bool *)calloc((size_t)(maxSum + 1), sizeof(*has));
    if (!has) return 0; /* allocation failure */

    /* Case i = 0 : sums j + k, 0 ≤ j,k ≤ n, gcd(j,k) ≤ 1 */
    for (idx_t j = 0; j <= n; ++j) {
        for (idx_t k = j; k <= n; ++k) {        /* symmetric; half loops */
            if (gcd_u64(j, k) <= 1)
                has[j + k] = true;
        }
    }

    /* Case i > 0, j = 0 : sums i + k, 1 ≤ i ≤ n, 0 ≤ k ≤ n, gcd(i,k) ≤ 1 */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = 0; k <= n; ++k) {
            if (gcd_u64(i, k) <= 1)
                has[i + k] = true;
        }
    }

    /* Case i > 0, j > 0 */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            idx_t g = gcd_u64(i, j);          /* gcd(i,j) */
            /* k = 0 case */
            if (g <= 1)
                has[i + j] = true;

            idx_t prod = i * j;                /* i·j ≤ 10⁸ for n ≤ 10⁴ */
            if (prod == 0 || prod > n)
                continue;                       /* only k = 0 possible, already handled */

            idx_t kmax = n / prod;
            for (idx_t k = 1; k <= kmax; ++k) {
                if (gcd_u64(g, k) <= 1)
                    has[i + j + k] = true;
            }
        }
    }

    seq_t count = 0;
    for (idx_t s = 0; s <= maxSum; ++s)
        if (has[s]) ++count;

    free(has);
    return count;
}
