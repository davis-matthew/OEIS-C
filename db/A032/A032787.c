/*
 * A032787  Numbers k such that k(k+1)(k+2)...(k+17) / (k+(k+1)+(k+2)+ ...
 *          +(k+17)) is an integer.
 * https://oeis.org/A032787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 549, u32 n <= 2539, u64 n <= 2827,
 *            u128 n <= 2827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032787: numbers k such that k(k+1)...(k+17)/(k+...+k+17) is an integer.
   The values are obtained from the formula:  k = (d-17)/2 for each divisor d≥17 of
   N = 131939107925625.  The sequence is finite.  The implementation generates
   all divisors of N, filters them, computes the corresponding k's, sorts them
   and stores the result for future queries. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparison function for uint64_t values used by qsort. */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static storage for the precomputed sequence. */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static idx_t length = 0;

    if (!initialized) {
        /* Constant N from OEIS comment. */
        const uint64_t N = 131939107925625ULL;

        /* ----------------------------------------------------
           1. Factorise N. */
        uint64_t tmp = N;
        const size_t max_factors = 64;
        uint64_t primes[max_factors];
        idx_t exponents[max_factors];
        idx_t primes_cnt = 0;

        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                uint64_t q = p;
                idx_t e = 0;
                while (tmp % q == 0) {
                    tmp /= q;
                    ++e;
                }
                primes[primes_cnt] = q;
                exponents[primes_cnt] = e;
                ++primes_cnt;
            }
        }
        if (tmp > 1) {
            primes[primes_cnt] = tmp;
            exponents[primes_cnt] = 1;
            ++primes_cnt;
        }

        /* ----------------------------------------------------
           2. Generate all divisors of N (uint64_t array). */
        size_t div_cap = 64;
        size_t div_len = 1;
        uint64_t *divs_u = (uint64_t *)malloc(div_cap * sizeof(uint64_t));
        divs_u[0] = 1;

        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            idx_t e = exponents[i];
            size_t old_len = div_len;
            uint64_t p_pow = 1;
            for (idx_t k = 1; k <= e; ++k) {
                p_pow *= p;
                for (size_t j = 0; j < old_len; ++j) {
                    uint64_t val = divs_u[j] * p_pow;
                    if (div_len == div_cap) {
                        div_cap *= 2;
                        divs_u = (uint64_t *)realloc(divs_u, div_cap * sizeof(uint64_t));
                    }
                    divs_u[div_len++] = val;
                }
            }
        }

        /* ----------------------------------------------------
           3. Filter divisors d ≥ 17, compute k = (d-17)/2. */
        size_t k_cap = div_len;
        uint64_t *ks_u = (uint64_t *)malloc(k_cap * sizeof(uint64_t));
        size_t k_len = 0;

        for (size_t i = 0; i < div_len; ++i) {
            if (divs_u[i] >= 17ULL) {
                uint64_t k = (divs_u[i] - 17ULL) / 2ULL;
                ks_u[k_len++] = k;
            }
        }

        free(divs_u);

        /* ----------------------------------------------------
           4. Sort the k-values. */
        qsort(ks_u, k_len, sizeof(uint64_t), cmp_uint64);

        /* ----------------------------------------------------
           5. Convert to seq_t array for public access. */
        terms = (seq_t *)malloc(k_len * sizeof(seq_t));
        for (size_t i = 0; i < k_len; ++i) {
            terms[i] = (seq_t)ks_u[i];
        }

        free(ks_u);

        length = k_len;
        initialized = true;
    }

    /* ----------------------------------------------------
       Return the nth term (1-based).  For n out of range
       the sequence is finite, so return 0. */
    if (n == 0) return (seq_t)0;     /* undefined for n=0, but guard against it */

    idx_t idx = n - 1;
    if (idx < length) {
        return terms[idx];
    } else {
        return (seq_t)0;
    }
}
