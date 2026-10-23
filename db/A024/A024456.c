/*
 * A024456  [ (4th elementary symmetric function of P(n))/(2nd elementary
 *          symmetric function of P(n)) ], where P(n) = {first n+3 primes}.
 * https://oeis.org/A024456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e4(P(n)) / e2(P(n)) where P(n) = set of the first n+3
 *            primes, e_k denotes the k‑th elementary symmetric function of
 *            its arguments
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Utility to estimate an upper bound for the k-th prime.
   For k >= 6, p_k < k (log k + log log k).  The function adds a small
   safety margin.  For very small k it returns a fixed small bound. */
static uint64_t _prime_upper_bound(uint64_t k)
{
    if (k < 6) return 15;
    double dk = (double)k;
    double ub = dk * (log(dk) + log(log(dk)));
    return (uint64_t)(ub + 10.0);      /* safety margin */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for 0, return 0 */

    /* Number of primes required for this call */
    idx_t req_k = n + 3;

    /* Static state persisting across calls */
    static uint64_t *primes = NULL;       /* array of primes (uint64_t)   */
    static size_t primes_cnt = 0;         /* number of primes stored     */
    static seq_t *S2 = NULL;              /* e2 values for 0..primes_cnt */
    static seq_t *S4 = NULL;              /* e4 values for 0..primes_cnt */
    static idx_t computed_k = 0;          /* primes processed so far     */
    static seq_t s1 = 0, s2 = 0, s3 = 0, s4 = 0; /* current symmetric sums */

    /* Ensure we have enough primes */
    if (primes == NULL ||
        (size_t)req_k > primes_cnt) {

        /* Determine a limit large enough for the required number of primes */
        uint64_t limit = _prime_upper_bound((uint64_t)req_k);

        /* Build the prime list – use the supplied sieve utility if available,
           otherwise fall back to a local implementation. */
#ifdef BUILD_PRIMES
        /* The environment provides a helper build_primes_upto */
        extern void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out);
        build_primes_upto(limit, &primes, &primes_cnt);
#else
        /* Simple Eratosthenes sieve that works for both C and C++ */
        bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
        size_t approx = (size_t)(limit / 10) + 10;
        uint64_t *tmp = (uint64_t*)malloc(approx * sizeof(uint64_t));
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                if (cnt == approx) {
                    approx *= 2;
                    tmp = (uint64_t*)realloc(tmp, approx * sizeof(uint64_t));
                }
                tmp[cnt++] = (uint64_t)i;
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i)
                        is_comp[j] = true;
                }
            }
        }
        free(is_comp);
        primes = tmp;
        primes_cnt = cnt;
#endif

        /* Allocate arrays for the symmetric sums */
        free(S2);
        free(S4);
        S2 = (seq_t*)malloc((primes_cnt + 1) * sizeof(seq_t));
        S4 = (seq_t*)malloc((primes_cnt + 1) * sizeof(seq_t));
        S2[0] = S4[0] = 0;

        /* Reset running sums and processed count */
        s1 = s2 = s3 = s4 = 0;
        computed_k = 0;
    }

    /* Extend precomputation up to req_k if necessary */
    if (computed_k < req_k) {
        for (size_t k = (size_t)computed_k; k < (size_t)req_k; ++k) {
            seq_t p = (seq_t)primes[k];          /* current prime as seq_t */

            /* Update elementary symmetric sums */
            s4 += s3 * p;
            s3 += s2 * p;
            s2 += s1 * p;
            s1 += p;

            /* Store cumulative values */
            S2[k + 1] = s2;
            S4[k + 1] = s4;
        }
        computed_k = req_k;
    }

    /* Return integer division e4/e2 (floor for positives). */
    return S4[req_k] / S2[req_k];
}
