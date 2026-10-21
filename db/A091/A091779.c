/*
 * A091779  G.f.: Product((1+x^i)/(1-x^i),i=1..n-1)/(1-x^n), with n = 8.
 * https://oeis.org/A091779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091779
   Formula:  a(n) = coefficient of  x^n  in
             ∏_{i=1}^{7} (1+x^i) / ∏_{i=1}^{8} (1-x^i)

   Recurrence:  a(0)=1,
               a(n) = p[n] - Σ_{k=1..min(n,36)} d_k · a[n-k]
   where  p[n] = coefficient of x^n in P(x)=∏_{i=1}^{7}(1+x^i)
          d_k  = coefficient of x^k in D(x)=∏_{i=1}^{8}(1-x^i)   (d_0=1)
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* coefficients of the denominator D(x) = ∏_{i=1}^{8}(1 - x^i)  (degree 36) */
static int64_t d_coeff[37] = {0};
/* coefficients of the numerator   P(x) = ∏_{i=1}^{7}(1 + x^i)  (degree 28) */
static int64_t p_coeff[29] = {0};
static int coeffs_built = 0;

/* build the coefficient tables once */
static void build_coeffs(void)
{
    if (coeffs_built) return;
    coeffs_built = 1;

    /* D(x) = ∏_{i=1}^{8} (1 - x^i) */
    int64_t cur[37] = {0};
    int deg = 0;
    cur[0] = 1;   /* start with 1 */
    for (int i = 1; i <= 8; ++i) {
        int next_deg = deg + i;
        int64_t nxt[37] = {0};
        for (int k = 0; k <= deg; ++k) {
            nxt[k]     += cur[k] *  1;   /* +1 * cur[k] */
            nxt[k + i] += cur[k] * -1;   /* -1 * cur[k] for x^i */
        }
        for (int k = 0; k <= next_deg; ++k) cur[k] = nxt[k];
        deg = next_deg;
    }
    for (int i = 0; i <= 36; ++i) d_coeff[i] = cur[i];

    /* P(x) = ∏_{i=1}^{7} (1 + x^i) */
    int64_t curp[29] = {0};
    int degp = 0;
    curp[0] = 1;
    for (int i = 1; i <= 7; ++i) {
        int next_deg = degp + i;
        int64_t nxtp[29] = {0};
        for (int k = 0; k <= degp; ++k) {
            nxtp[k]     += curp[k] * 1;   /* +1 * curp[k] */
            nxtp[k + i] += curp[k] * 1;   /* +1 * curp[k] for x^i */
        }
        for (int k = 0; k <= next_deg; ++k) curp[k] = nxtp[k];
        degp = next_deg;
    }
    for (int i = 0; i <= 28; ++i) p_coeff[i] = curp[i];
}

/* memoised array for a(n) */
static seq_t *seq_vals = NULL;
static size_t seq_cap  = 0;       /* allocated size of seq_vals array   */
static idx_t  seq_max  = (idx_t)-1;   /* highest n that has been computed   */

/* multiply a seq_t by a small unsigned int */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return x * (seq_t)m;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    build_coeffs();

    /* allocate space for memoisation if necessary */
    if (seq_cap < 1) {
        seq_cap = 32;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_vals) exit(1);
    }
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_vals = tmp;
        seq_cap = new_cap;
    }

    /* initialise the first value if this is the first call */
    if (seq_max == (idx_t)-1) {
        seq_vals[0] = (seq_t)p_coeff[0];   /* p_coeff[0] == 1 */
        seq_max = 0;
    }

    if ((idx_t)n <= seq_max)   /* already computed */
        return seq_vals[(size_t)n];

    /* compute coefficients up to n */
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        /* compute Σ_{k=1..min(i,36)} d_k * a[i-k] */
        seq_t sum = 0;
        int maxk = (i < 36) ? (int)i : 36;
        for (int k = 1; k <= maxk; ++k) {
            seq_t term = mul_small(seq_vals[(size_t)(i - k)],
                                   (unsigned int)(d_coeff[k] > 0 ? d_coeff[k] : -d_coeff[k]));
            if (d_coeff[k] > 0)
                sum += term;
            else
                sum -= term;
        }
        seq_t p_val = (i <= 28) ? (seq_t)p_coeff[(size_t)i] : 0;
        seq_vals[(size_t)i] = p_val - sum;   /* d_0 == 1 */
    }
    seq_max = n;
    return seq_vals[(size_t)n];
}
