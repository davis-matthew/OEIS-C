/*
 * A238346  Triangle T(n,k) read by rows: T(n,k) is the number of compositions
 *          of n where the k-th part is the last occurrence of a largest part,
 *          n>=1, 1<=k<=n.
 * https://oeis.org/A238346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) by rows, * where
 *            T(n,k) counts the compositions of n whose k‑th part is the *
 *            last occurrence of the largest part. For n≥1, a(n)=T(Row,Col) *
 *            where Row and Col are derived from n by reading the triangle
 * Range:     u8 n <= 66, u16 n <= 210, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAXN 150            /* > maximum possible row (≈141 for n≤10000) */

static seq_t comb_table[MAXN+1][MAXN+1];
static bool comb_initialized = false;

/* Precompute binomial coefficients C[n][k] for 0≤n,k≤MAXN */
static void init_comb(void)
{
    if (comb_initialized) return;
    for (int n = 0; n <= MAXN; ++n) {
        for (int k = 0; k <= n; ++k) {
            if (k == 0 || k == n)
                comb_table[n][k] = 1;
            else
                comb_table[n][k] = comb_table[n-1][k-1] + comb_table[n-1][k];
        }
    }
    comb_initialized = true;
}

/* Compute T(row,col) – the triangle entry.
 * row and col are 1‑based indices, and row ≤ MAXN. */
static seq_t compute_T_row_col(idx_t row, idx_t col)
{
    init_comb();

    int N = (int)row;          /* the value n in the definition */
    int L = (int)(col - 1);    /* number of parts before the last largest part */

    /* f[p][t] = number of compositions of t with parts ≤ p (p=0…N‑1, t=0…N) */
    seq_t f[MAXN][MAXN+1];
    for (int t = 0; t <= N; ++t)
        f[0][t] = (t == 0) ? 1 : 0;

    for (int p = 1; p <= N-1; ++p) {
        f[p][0] = 1;
        for (int t = 1; t <= N; ++t) {
            seq_t sum = 0;
            int maxd = (t < p) ? t : p;
            for (int d = 1; d <= maxd; ++d)
                sum += f[p][t-d];
            f[p][t] = sum;
        }
    }

    seq_t result = 0;

    for (int M = 1; M <= N; ++M) {
        int min_s1 = L;
        int max_s1 = N - M;
        if (min_s1 > max_s1) continue;

        for (int s1 = min_s1; s1 <= max_s1; ++s1) {
            seq_t G;
            if (L == 0) {
                G = (s1 == 0) ? 1 : 0;
            } else {
                seq_t pos = 0, neg = 0;
                int maxj = (s1 - L) / M;
                for (int j = 0; j <= maxj; ++j) {
                    /* C(L,j) * C(s1 - j*M - 1, L-1) */
                    seq_t term = comb_table[L][j] *
                                 comb_table[s1 - j*M - 1][L-1];
                    if ((j & 1) == 0)
                        pos += term;
                    else
                        neg += term;
                }
                G = pos - neg;           /* pos ≥ neg */
            }
            if (G == 0) continue;

            int suffix_t = N - M - s1;
            seq_t suffix = f[M-1][suffix_t];
            if (suffix == 0) continue;

            result += G * suffix;
        }
    }

    return result;
}

/* Main function required by the sequence definition */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 for safety */

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev = ((r - 1) * r) / 2;
    idx_t col  = n - prev;          /* column within the found row */

    return compute_T_row_col(r, col);
}
