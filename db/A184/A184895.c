/*
 * A184895  a(n) = (7^n/n!^2) * Product_{k=0..n-1} (14k+1)*(14k+6).
 * https://oeis.org/A184895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184895: a(n) = (7^n / (n!^2)) * ∏_{k=0}^{n-1} (14k+1)(14k+6) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Utility functions from the repository */
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Special small case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Upper bound for primes needed */
    idx_t max_val = 14 * n + 6;

    /* Build list of primes up to max_val */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(max_val, &prime_list, &prime_cnt);
    if (!prime_list) {
        return (seq_t)0; /* Should not happen */
    }

    /* Map each prime value to its index for O(1) lookup */
    int *prime_index = (int *)malloc((max_val + 1) * sizeof(int));
    if (!prime_index) {
        free(prime_list);
        return (seq_t)0;
    }
    memset(prime_index, -1, (max_val + 1) * sizeof(int));
    for (size_t i = 0; i < prime_cnt; ++i) {
        prime_index[prime_list[i]] = (int)i;
    }

    /* Exponent array for each prime (signed to allow subtraction later) */
    int64_t *expon = (int64_t *)calloc(prime_cnt, sizeof(int64_t));
    if (!expon) {
        free(prime_index);
        free(prime_list);
        return (seq_t)0;
    }

    /* 7^n contributes n times to prime 7 */
    {
        int idx7 = prime_index[7];
        if (idx7 >= 0) {
            expon[idx7] += (int64_t)n;
        }
    }

    /* Temporary buffers for factorization */
    uint64_t prim_buf[64];
    uint64_t exp_buf[64];
    size_t cnt_buf;

    /* Process all k from 0 to n-1 */
    for (idx_t k = 0; k < n; ++k) {
        idx_t v1 = 14 * k + 1;
        idx_t v2 = 14 * k + 6;

        /* Factor v1 */
        factor_u64((uint64_t)v1, prim_buf, exp_buf, &cnt_buf);
        for (size_t j = 0; j < cnt_buf; ++j) {
            int idx = prime_index[(int)prim_buf[j]];
            if (idx >= 0) {
                expon[idx] += (int64_t)exp_buf[j];
            }
        }
        /* Factor v2 */
        factor_u64((uint64_t)v2, prim_buf, exp_buf, &cnt_buf);
        for (size_t j = 0; j < cnt_buf; ++j) {
            int idx = prime_index[(int)prim_buf[j]];
            if (idx >= 0) {
                expon[idx] += (int64_t)exp_buf[j];
            }
        }
    }

    /* Subtract (n!^2) exponents: 2 * exponent of each prime in n! */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        /* exponent of p in n! */
        idx_t pp = p;
        int64_t e_fac = 0;
        while (pp <= n) {
            e_fac += n / pp;
            if (pp > n / p) break;
            pp *= p;
        }
        expon[pi] -= 2 * e_fac;
        /* After subtraction, expon[pi] should be >= 0 */
        /* It is guaranteed by the definition. */
    }

    /* Assemble the final result */
    seq_t result = seq_one();
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        if (expon[pi] > 0) {
            seq_t pow_val = seq_pow((seq_t)prime_list[pi], (idx_t)expon[pi]);
            result = seq_mul(result, pow_val);
        }
    }

    /* Clean up */
    free(prime_index);
    free(prime_list);
    free(expon);

    return result;
}
