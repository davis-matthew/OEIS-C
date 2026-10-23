/*
 * A145155  Coefficients in expansion of Delta'(q).
 * https://oeis.org/A145155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+1) * tau(n+1)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 27, i64 n <= 785, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where tau(n) is the Ramanujan tau function, coefficients of Δ(q). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *tau_cache = NULL;      /* tau[0..] */
static idx_t  cache_max = 0;         /* largest index computed so far */

/* Precomputed coefficients of (1 - x^m)^24 = Σ_{j=0}^{24} (-1)^j C(24,j) x^{jm} */
static seq_t coeff[25];
static int coeff_ready = 0;

/* Compute the binomial coefficients and sign */
static void
build_coeff(void)
{
    if (coeff_ready) return;
    coeff[0] = 1;
    unsigned long long c = 1;           /* C(24,0) */
    for (int j = 1; j <= 24; ++j) {
        /* c = c * (24 - j + 1) / j */
        c = c * (24 - j + 1) / j;
        coeff[j] = ((j & 1) == 0) ? (seq_t)c : -(seq_t)c;
    }
    coeff_ready = 1;
}

/* Ensure tau_cache contains tau[0..N] */
static void
ensure_tau(idx_t N)
{
    if (N <= cache_max) return;

    build_coeff();
    size_t size = (size_t)(N + 1);
    seq_t *dp_cur  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(size, sizeof(seq_t));

    /* Δ(q) = q * ∏_{m≥1} (1 - q^m)^24
       Start with the polynomial q */
    dp_cur[1] = 1;     /* τ(1) = 1 */

    for (idx_t m = 1; m <= N; ++m) {
        /* Zero dp_next for this stage */
        memset(dp_next, 0, sizeof(seq_t) * size);

        for (idx_t n = 0; n <= N; ++n) {
            seq_t val = dp_cur[n];
            if (!val) continue;

            /* For j such that n + j*m <= N */
            for (int j = 0; j <= 24; ++j) {
                idx_t idx = n + j * m;
                if (idx > N) break;
                dp_next[idx] += val * coeff[j];
            }
        }

        /* Prepare for next m */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* dp_cur now holds τ[0..N] */
    if (tau_cache) free(tau_cache);
    tau_cache = dp_cur;
    cache_max = N;

    /* dp_next is no longer needed */
    free(dp_next);
}

/* Public interface */
seq_t a(idx_t n)
{
    idx_t need = n + 1;          /* compute tau up to this index */
    ensure_tau(need);
    return (seq_t)(n + 1) * tau_cache[need];
}
