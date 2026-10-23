/*
 * A103353  First column of triangular matrix A103244.
 * https://oeis.org/A103353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1 a(n) = sum_{k=1}^{n-1} (-1)^(n-k+1) * C(n-1,k-1)
 *            * a(k) * (k^2+k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_vals = NULL;   /* stores a(1), a(2), …   */
static seq_t *pow_vals = NULL; /* pow_vals[k] = (k^2+k)^(current_n - k)   */
static idx_t a_cap   = 0;
static idx_t pow_cap = 0;
static idx_t computed = 0;     /* number of terms already computed   */

static void ensure_a(idx_t new_cap)
{
    if (new_cap <= a_cap) return;
    seq_t *tmp = (seq_t*)malloc(sizeof(seq_t)*new_cap);
    if (!tmp) return;
    if (a_vals) {
        memcpy(tmp, a_vals, sizeof(seq_t)*a_cap);
        free(a_vals);
    } else {
        memset(tmp, 0, sizeof(seq_t)*new_cap);
    }
    for (idx_t i = a_cap; i < new_cap; ++i) tmp[i] = 0;
    a_vals = tmp;
    a_cap  = new_cap;
}

static void ensure_pow(idx_t new_cap)
{
    if (new_cap <= pow_cap) return;
    seq_t *tmp = (seq_t*)malloc(sizeof(seq_t)*new_cap);
    if (!tmp) return;
    if (pow_vals) {
        memcpy(tmp, pow_vals, sizeof(seq_t)*pow_cap);
        free(pow_vals);
    } else {
        memset(tmp, 0, sizeof(seq_t)*new_cap);
    }
    for (idx_t i = pow_cap; i < new_cap; ++i) tmp[i] = 0;
    pow_vals = tmp;
    pow_cap  = new_cap;
}

seq_t a(idx_t n)   /* 1‑based index */
{
    if (n == 0) return 0;   /* undefined, but safe */

    /* initialise for the first call */
    if (computed == 0) {
        ensure_a(1);
        a_vals[0] = (seq_t)1;   /* a(1) */
        computed = 1;
        ensure_pow(1);          /* pow_vals[0] unused, set to 0 */
    }

    /* if already computed, return directly */
    if (n <= computed) return a_vals[n-1];

    /* compute terms up to n */
    for (idx_t new_n = computed + 1; new_n <= n; ++new_n) {

        /* Ensure power array can hold indices up to new_n-1 */
        ensure_pow(new_n);

        /* Update existing powers and initialise the new one */
        for (idx_t k = 1; k < new_n; ++k) {
            seq_t base = (seq_t)(k*k + k);   /* k^2 + k  */
            if (k <= new_n - 2) {                  /* existed before  */
                pow_vals[k] = pow_vals[k] * base;
            } else {                               /* new k  */
                pow_vals[k] = base;
            }
        }

        /* Compute a(new_n) using the recurrence */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        seq_t bin = 1; /* C(new_n-1,0) */

        for (idx_t k = 1; k < new_n; ++k) {
            seq_t term = bin * a_vals[k-1] * pow_vals[k];
            if (((new_n - k + 1) & 1) == 0) {
                sum_pos += term;   /* even exponent -> + */
            } else {
                sum_neg += term;   /* odd  exponent -> - */
            }
            /* update binomial coefficient for next k */
            bin = bin * (seq_t)(new_n - k) / (seq_t)k;
        }

        seq_t val = sum_pos - sum_neg;   /* guaranteed non‑negative */
        ensure_a(new_n);
        a_vals[new_n-1] = val;
        computed = new_n;
    }

    return a_vals[n-1];
}
