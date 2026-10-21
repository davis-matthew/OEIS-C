/*
 * A090582  T(n, k) = Sum_{j=0..n-k} (-1)^j*binomial(n - k + 1, j)*(n - k + 1
 *          - j)^n. Triangle read by rows, T(n, k) for 1 <= k <= n.
 * https://oeis.org/A090582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8n+1) – 1)/2⌋ (the row of the
 *            triangle) c = n – r·(r–1)/2 (the column) and T(r,c) = (r–c+1)! ·
 *            S(r, r–c+1) with S(r,m) the Stirling numbers of the second kind
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 68, u64 n <= 176,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ----------  temporary array for Stirling numbers  ---------- */
static seq_t *S = NULL;     /* S[0..m]  (allocated on demand) */
static idx_t S_cap = 0;     /* current capacity of S */

static void ensure_S_cap(idx_t cap)
{
    if (cap <= S_cap)
        return;
    idx_t new_cap = (S_cap == 0) ? cap : S_cap * 2;
    if (new_cap < cap)
        new_cap = cap;
    seq_t *tmp = (seq_t *)realloc(S, (new_cap + 1) * sizeof(seq_t));
    S = tmp;
    S_cap = new_cap;
}

/* ----------  compute T(r,c)  ---------- */
static seq_t compute_T(idx_t r, idx_t c)
{
    idx_t m = r - c + 1;            /* m = r‑c+1 = n‑k+1 */
    if (m == 0)
        return (seq_t)0;            /* shouldn't happen */
    if (m == 1)                    /* T(r,r) = 1! · S(r,1) = 1 */
        return (seq_t)1;

    /*  compute S(r,m) by the recurrence
        S(1,1)=1, S(i,0)=S(0,j)=0            */
    ensure_S_cap(m);
    for (idx_t j = 0; j <= m; ++j)
        S[j] = (seq_t)0;
    S[1] = (seq_t)1;

    for (idx_t i = 2; i <= r; ++i) {
        idx_t upper = (i < m) ? i : m;
        /* update from high to low to reuse same array */
        for (idx_t j = upper; j >= 1; --j) {
            S[j] = (seq_t)j * S[j] + S[j - 1];
        }
    }

    seq_t stirling = S[m];

    /* factorial(m) */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    return fact * stirling;
}

/* ----------  public function a(idx_t n)  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – never used in tests */
        return (seq_t)0;

    /* locate row r and column c */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);   /* floor */
    if (r * (r + 1) / 2 < n)
        r++;           /* adjust if necessary */

    idx_t c = n - r * (r - 1) / 2;   /* 1‑based column */

    return compute_T(r, c);
}
