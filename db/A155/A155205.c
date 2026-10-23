/*
 * A155205  G.f.: A(x) = exp( Sum_{n>=1} (3^n - 1)^n * x^n/n ), a power series
 *          in x with integer coefficients.
 * https://oeis.org/A155205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{k>=1} ((3^k - 1)^k * x^k / k) ), a(0)=1
 *            Recurrence: a(n) = (1/n) * sum_{k=1}^n (3^k - 1)^k * a(n-k)
 *            (n>=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* exponentiation by squaring for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* caches for the sequence and auxiliary values */
static seq_t *a_cache   = NULL; /* a(n)          */
static seq_t *pow3_cache = NULL; /* 3^n           */
static seq_t *c_cache   = NULL; /* (3^n-1)^n     */
static size_t a_cap = 0;        /* allocated size */
static size_t a_size = 0;       /* number of values ready (indices 0..a_size-1) */

/* ensure that values up to n are computed */
static void ensure_caches(idx_t n) {
    if ((size_t)n + 1 <= a_size) return;               /* already done */

    size_t needed = (size_t)n + 1;
    if (needed > a_cap) {                              /* grow storage */
        size_t new_cap = a_cap ? a_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_a   = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *new_p3  = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *new_c   = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (a_cache) {
            for (size_t i = 0; i < a_size; ++i) {
                new_a[i]   = a_cache[i];
                new_p3[i]  = pow3_cache[i];
                new_c[i]   = c_cache[i];
            }
            free(a_cache);   free(pow3_cache); free(c_cache);
        }
        a_cache     = new_a;
        pow3_cache  = new_p3;
        c_cache     = new_c;
        a_cap = new_cap;
    }

    /* compute missing entries */
    for (size_t idx = a_size; idx < needed; ++idx) {
        if (idx == 0) {                    /* base cases */
            a_cache[0] = (seq_t)1;
            pow3_cache[0] = (seq_t)1;
            c_cache[0] = (seq_t)0;
            continue;
        }
        pow3_cache[idx] = pow3_cache[idx-1] * (seq_t)3;
        seq_t base = pow3_cache[idx];
        base -= (seq_t)1;
        c_cache[idx] = pow_seq(base, (idx_t)idx);

        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= idx; ++k) {
            sum += c_cache[k] * a_cache[idx-k];
        }
        a_cache[idx] = sum / (seq_t)idx;
    }
    a_size = needed;             /* now a[0..n] are ready */
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_caches(n);
    return a_cache[(size_t)n];
}
