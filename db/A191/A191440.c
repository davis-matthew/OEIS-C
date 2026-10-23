/*
 * A191440  Dispersion of ([n*sqrt(2)+n+3/2]), where [ ]=floor, by
 *          antidiagonals.
 * https://oeis.org/A191440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the dispersion array of * s(n) =
 *            floor(n*sqrt(2)+n+3/2) traversed by anti‑diagonals
 * Range:     u8 none, u16 n <= 15, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SQRT2_P 1393ULL      /* 1393/985 ≈ 1.414213538... */
#define SQRT2_Q 985ULL
#define M (SQRT2_P + SQRT2_Q)   /* 2378 */

/* Compute s(n) = floor(n*sqrt(2)+n+3/2) with integer arithmetic. */
static inline seq_t compute_s(seq_t n)
{
    seq_t prod = n * (seq_t)M;
    seq_t base = prod / (seq_t)SQRT2_Q;
    seq_t rem  = prod % (seq_t)SQRT2_Q;
    seq_t result = base + 1;
    if ((rem << 1) >= (seq_t)SQRT2_Q)
        result += 1;
    return result;
}

/* ----------  on‑demand construction of t and s ---------- */
static seq_t *t_vals      = NULL;
static size_t  t_cap      = 0;
static size_t  t_len      = 0;

static idx_t   s_idx      = 1;   /* next index of s to compute */
static seq_t   s_value    = 0;   /* current value of s(s_idx)     */
static seq_t   m_cur      = 1;   /* current integer being scanned */

/* Ensure t_vals has at least 'needed' elements. */
static void ensure_t_has(idx_t needed)
{
    while (t_len < (size_t)needed) {
        /* Scan until one new t value is found. */
        while (true) {
            if (m_cur == s_value) {
                /* m_cur belongs to s: skip it. */
                ++s_idx;
                s_value = compute_s((seq_t)s_idx);
                ++m_cur;
            } else {
                /* m_cur is a member of t.  Store it. */
                if (t_len == t_cap) {
                    t_cap = t_cap ? t_cap << 1 : 8;
                    seq_t *tmp = (seq_t *)realloc(t_vals, t_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    t_vals = tmp;
                }
                t_vals[t_len++] = m_cur;
                ++m_cur;
                break;
            }
        }
    }
}

/* ----------  main sequence function ---------- */
idx_t initialized = 0;
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n=0 */

    /* Lazy initialization of static data. */
    if (!initialized) {
        initialized = 1;
        s_idx   = 1;
        s_value = compute_s((seq_t)s_idx);
        m_cur   = 1;
        t_len   = 0;
        t_cap   = 0;
        t_vals  = NULL;
    }

    /* Find diagonal index d: smallest with d*(d+1)/2 >= n. */
    idx_t d = 1;
    for (;;) {
        unsigned __int128 tri = (unsigned __int128)d * (d + 1) / 2;
        if (tri >= n) break;
        ++d;
    }

    /* Offsetting within the diagonal. */
    unsigned __int128 tri_prev128 = (unsigned __int128)(d - 1) * d / 2;
    idx_t tri_prev = (idx_t)tri_prev128;
    idx_t offset = n - tri_prev - 1;     /* zero‑based offset in diagonal */

    /* Row (i) and column (j) of the element. */
    idx_t i = offset + 1;                /* row index (1‑based) */
    idx_t j = d + 1 - i;                 /* column index */

    /* Ensure we have the requested t(i). */
    ensure_t_has(i);

    /* Compute D[i, j] = apply s j‑1 times starting from t(i). */
    seq_t val = t_vals[i - 1];
    for (idx_t k = 0; k < j - 1; ++k)
        val = compute_s(val);

    return val;
}
