/*
 * A099391  Expansion of e.g.f. 1/(2 - exp(exp(exp(x) - 1) - 1)).
 * https://oeis.org/A099391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A099391
 *  a(n) = number of set‑partitions of a set of size n whose blocks are
 *  themselves sets of nonempty sets.
 *  Recurrence:
 *      a(0) = 1
 *      a(n) = G(n) + Σ_{k=1}^{n-1} binom(n,k) * a(k) * G(n-k)
 *
 *  where G(n) = Σ_{j=1}^{n} S(n,j) * Bell(j)
 *  and S(n,j) are Stirling numbers of the second kind,
 *  Bell(j) are Bell numbers.
 *
 *  The implementation computes the required Stirling numbers,
 *  Bell numbers, G(n) and finally a(n) using only integer operations.
 *  It uses memoization so calls can be made in any order.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Local helper: multiply then divide, guaranteeing exact
 *  division.  The divisor always divides the product in our
 *  algorithm, so truncation is safe. */
static inline seq_t mul_div(seq_t x, seq_t y, seq_t d) {
    /* Avoid overflow on intermediate product when possible */
    return (x * y) / d;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static memoised state */
    static bool initialized = false;
    static idx_t cache_size = 0;
    static seq_t *a_cache    = NULL;   /* a[0..cache_size]   */
    static seq_t *g_cache    = NULL;   /* G[0..cache_size]   */
    static seq_t *bell_cache = NULL;   /* Bell[0..cache_size]*/
    static seq_t *s_prev     = NULL;   /* previous Stirling row */

    if (!initialized || n > cache_size) {
        idx_t max_n = n;

        /* allocate / resize arrays --------------------------------- */
        a_cache    = (seq_t *)realloc(a_cache,    (max_n + 1) * sizeof(seq_t));
        g_cache    = (seq_t *)realloc(g_cache,    (max_n + 1) * sizeof(seq_t));
        bell_cache = (seq_t *)realloc(bell_cache, (max_n + 1) * sizeof(seq_t));
        s_prev     = (seq_t *)realloc(s_prev,     (max_n + 1) * sizeof(seq_t));

        if (!a_cache || !g_cache || !bell_cache || !s_prev)
            abort();

        /* ---------------------------------------------------------- */
        /* 1. Bell numbers   (Bell[0] = 1) --------------------------- */
        bell_cache[0] = 1;
        for (idx_t i = 1; i <= max_n; ++i) {
            seq_t sum  = 0;
            seq_t binom = 1;                 /* binom(i-1,0) */
            for (idx_t k = 0; k <= i-1; ++k) {
                sum += binom * bell_cache[k];
                if (k != i-1) {
                    binom = binom * ((i - 1) - k);
                    binom = binom / (k + 1);
                }
            }
            bell_cache[i] = sum;
        }

        /* ---------------------------------------------------------- */
        /* 2. Compute a(n) and G(n) row by row ---------------------- */
        /* initial Stirling row for n = 0: S(0,0) = 1 */
        for (idx_t i = 0; i <= max_n; ++i) s_prev[i] = 0;
        s_prev[0] = 1;

        /* G(0), a(0) ---------------------------------------------- */
        g_cache[0] = bell_cache[0] * s_prev[0];   /* 1*1 = 1          */
        a_cache[0] = g_cache[0];                  /* 1                */

        /* buffer for the new Stirling row */
        seq_t *s_new = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
        if (!s_new) abort();

        for (idx_t cur = 1; cur <= max_n; ++cur) {
            /* ---- Stirling numbers for current n ------------------ */
            s_new[0] = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                /* S(cur,k) = S(cur-1,k-1) + k * S(cur-1,k) */
                seq_t term = s_prev[k-1];
                if (k <= cur-1) term += mul_div(k, s_prev[k], 1); /* k * s_prev[k] */
                s_new[k] = term;
            }

            /* ---- G(cur) = Σ_k S(cur,k) * Bell(k) ---------------- */
            seq_t G_cur = 0;
            for (idx_t k = 1; k <= cur; ++k)
                G_cur += s_new[k] * bell_cache[k];
            g_cache[cur] = G_cur;

            /* ---- a(cur) = G(cur) + Σ_{i=1}^{cur-1} binom(cur,i)
             *           * a(i) * G(cur-i) ------------------------
             */
            seq_t a_cur = G_cur;
            seq_t binom = 1;          /* binom(cur,0) = 1 */
            for (idx_t i = 1; i <= cur-1; ++i) {
                binom = mul_div((seq_t)(cur - i + 1), binom, 1); /* update binom(cur,i) */
                binom = mul_div(binom, 1, (seq_t)i);          /* exact division */
                a_cur += mul_div(binom, a_cache[i] * g_cache[cur - i], 1);
            }
            a_cache[cur] = a_cur;

            /* ---- prepare for next iteration --------------------- */
            for (idx_t k = 0; k <= cur; ++k)
                s_prev[k] = s_new[k];
        }

        free(s_new);
        cache_size = max_n;
        initialized = true;
    }

    return a_cache[n];
}
