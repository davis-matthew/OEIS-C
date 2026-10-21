/*
 * A055942  Smallest k > 1 such that k | Sigma(k) + n.
 * https://oeis.org/A055942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Smallest k > 1 such that k | Sigma(k) + n
 * Range:     u8 n <= 507, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Precompute the divisor‑sum function sigma(k) up to a fixed limit.
 * The limit is chosen large enough for all n up to 10 000.
 * For any larger n the code will continue searching beyond the limit;
 * however the required k never exceeds the chosen limit in practice.
 */
#define SIGMA_LIMIT 500000U

static seq_t *sigma_array = NULL;  /* sigma(k) values, 0 ≤ k ≤ SIGMA_LIMIT */

/* Initialize sigma_array once. */
static void init_sigma(void) {
    if (sigma_array != NULL) return;          /* already initialized */

    sigma_array = (seq_t *)malloc((SIGMA_LIMIT + 1) * sizeof(seq_t));
    if (sigma_array == NULL) {
        /* Allocation failed – abort program. */
        abort();
    }

    /* Set all to zero. */
    for (idx_t i = 0; i <= SIGMA_LIMIT; ++i) {
        sigma_array[i] = (seq_t)0;
    }

    /* Standard divisor sieve: add i to all multiples of i. */
    for (idx_t i = 1; i <= SIGMA_LIMIT; ++i) {
        seq_t si = (seq_t)i;                  /* cast once */
        for (idx_t j = i; j <= SIGMA_LIMIT; j += i) {
            sigma_array[j] += si;
        }
    }
}

seq_t a(idx_t n) {
    /* Ensure sigma has been computed. */
    init_sigma();

    seq_t n_seq = (seq_t)n;
    for (idx_t k = 2; ; ++k) {
        seq_t sigma_k;
        if (k <= SIGMA_LIMIT) {
            sigma_k = sigma_array[k];
        } else {
            /* k is larger than precomputed limit – compute sigma on the fly. */
            sigma_k = (seq_t)1;              /* sigma(1) always 1 */
            idx_t kk = k;
            /* Simple trial division up to sqrt(k). */
            for (idx_t d = 2; d * d <= kk; ++d) {
                if (kk % d == 0) {
                    /* d is a prime divisor with exponent e. */
                    seq_t term = (seq_t)1;
                    seq_t power = (seq_t)1;
                    while (kk % d == 0) {
                        kk /= d;
                        power *= (seq_t)d;
                        term += power;
                    }
                    sigma_k *= term;
                }
            }
            if (kk > 1) {
                /* Remaining prime factor. */
                sigma_k *= ((seq_t)1 + (seq_t)kk);
            }
        }

        /* Check the divisibility condition. */
        if ((sigma_k + n_seq) % (seq_t)k == (seq_t)0) {
            return (seq_t)k;
        }
    }
}
