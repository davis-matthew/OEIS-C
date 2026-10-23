/*
 * A154602  Exponential Riordan array [exp(sinh(x)*exp(x)), sinh(x)*exp(x)].
 * https://oeis.org/A154602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2) and c = n -
 *            r(r+1)/2. T(r,c) = Σ_{i=c..r} 2^(r-i)·C(i,c)·S(r,i), with S(r,i)
 *            the Stirling numbers of the second kind
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 79, u64 n <= 231,
 *            u128 n <= 632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- helper: integer square root of 64‑bit unsigned integers ---------- */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long low = 0, high = x;
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        if ( (__int128)mid * mid <= x )
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- tables for binomial coefficients, Stirling2, powers of two --------- */
static idx_t max_row = 0;          /* largest computed row index */
static seq_t **Stirling = NULL;    /* Stirling[n][k]  (0 ≤ k ≤ n) */
static seq_t **Binom    = NULL;    /* Binom[n][k]     (0 ≤ k ≤ n)   */
static seq_t *pow2      = NULL;    /* pow2[i] = 2^i   */
static idx_t pow2_len   = 0;        /* number of entries in pow2 */

/* initialise tables for row 0 */
static void init_tables(void)
{
    Stirling = (seq_t**)calloc(1, sizeof(seq_t*));
    Binom    = (seq_t**)calloc(1, sizeof(seq_t*));
    pow2     = (seq_t*)calloc(1, sizeof(seq_t));
    if (!Stirling || !Binom || !pow2) return;           /* ignore allocation failure */

    /* row 0 */
    Stirling[0] = (seq_t*)malloc(sizeof(seq_t));
    Stirling[0][0] = (seq_t)1;   /* S(0,0) = 1 */

    Binom[0] = (seq_t*)malloc(sizeof(seq_t));
    Binom[0][0] = (seq_t)1;      /* C(0,0) = 1 */

    pow2[0] = (seq_t)1;          /* 2^0 = 1 */
    max_row   = 0;
    pow2_len  = 1;
}

/* extend tables up to new_max (inclusive) */
static void extend_tables(idx_t new_max)
{
    if (new_max <= max_row) return;

    /* enlarge pointer arrays */
    Stirling = (seq_t**)realloc(Stirling, sizeof(seq_t*) * (new_max + 1));
    Binom    = (seq_t**)realloc(Binom,    sizeof(seq_t*) * (new_max + 1));

    /* allocate new rows and initialise borders */
    for (idx_t r = max_row + 1; r <= new_max; ++r) {
        Stirling[r] = (seq_t*)malloc(sizeof(seq_t) * (r + 1));
        Binom[r]    = (seq_t*)malloc(sizeof(seq_t) * (r + 1));

        Stirling[r][0] = (seq_t)0;   /* S(r,0) = 0 for r>0 */
        Binom[r][0]    = (seq_t)1;   /* C(r,0) = 1 */
        Binom[r][r]    = (seq_t)1;   /* C(r,r) = 1 */
    }

    /* compute Stirling and binomial values for the new rows */
    for (idx_t r = max_row + 1; r <= new_max; ++r) {
        /* binomial coefficients */
        for (idx_t k = 1; k < r; ++k)
            Binom[r][k] = Binom[r-1][k-1] + Binom[r-1][k];

        /* Stirling numbers of the second kind */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t term1 = 0, term2 = 0;
            if (k <= r-1) term1 = Stirling[r-1][k] * (seq_t)k;
            if (k-1 <= r-1) term2 = Stirling[r-1][k-1];
            Stirling[r][k] = term1 + term2;
        }
    }
    max_row = new_max;

    /* extend powers of two if needed */
    if (pow2_len <= new_max) {
        pow2 = (seq_t*)realloc(pow2, sizeof(seq_t) * (new_max + 1));
        if (pow2_len == 0) {
            pow2[0] = (seq_t)1;
            pow2_len = 1;
        }
        for (idx_t i = pow2_len; i <= new_max; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
        pow2_len = new_max + 1;
    }
}

/* --------------------------------------------------------------------------- */
/* The requested OEIS sequence function                                         */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise tables on first call */
    if (!Stirling) init_tables();

    /* locate the row r and column c for the flattened index n */
    idx_t r, c, start;
    if (n == (idx_t)-1) { r = 0; c = 0; } else {
        /* solve r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
        unsigned long long t = 8ULL * n + 1ULL;
        unsigned long long s = isqrt_u64(t);
        r = (s - 1ULL) / 2ULL;
        /* fix possible off‑by‑one due to integer truncation */
        while ((r + 1) * (r + 2) / 2 <= n) ++r;
        while (r * (r + 1) / 2 > n) --r;
        start = r * (r + 1) / 2;
        c = n - start;
    }

    /* prepare tables for this row */
    extend_tables(r);

    /* compute T(r,c) = Σ_{i=c..r} 2^(r-i)·C(i,c)·S(r,i) */
    seq_t result = (seq_t)0;
    for (idx_t i = c; i <= r; ++i) {
        seq_t term = pow2[r - i];                /* 2^(r-i)   */
        term = term * Binom[i][c];               /* × C(i,c)   */
        term = term * Stirling[r][i];            /* × S(r,i)   */
        result = result + term;
    }
    return result;
}
