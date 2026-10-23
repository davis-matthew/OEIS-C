/*
 * A141553  Transformed nonprime products of prime factors of the composites,
 *          the largest prime decremented by 2 and the smallest prime
 *          incremented by 1.
 * https://oeis.org/A141553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For each composite k, let pmin and pmax be its * smallest and
 *            largest prime factors. Replace one occurrence * of pmax by
 *            (pmax-2) and one occurrence of pmin by (pmin+1), * then multiply
 *            all (modified) prime factors. If the product is * prime, the
 *            term is skipped. This function returns the nth * s...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_spf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static state for memoisation */
    static idx_t *spf = NULL;                /* smallest prime factor array */
    static idx_t spf_limit = 0;              /* maximum index filled in spf     */
    static idx_t next_k = 4;                 /* next composite to examine       */
    static seq_t *output = NULL;   /* produced sequence values         */
    static size_t out_len = 0, out_cap = 0;   /* current length and capacity     */

    /* initialise the spf array with a modest limit if needed */
    if (spf == NULL) {
        spf_limit = 100;
        build_spf(spf_limit, &spf);          /* allocates turnpike from 0..spf_limit */
    }

    /* generate terms until we have at least n of them */
    while (out_len < n) {
        /* ensure spf covers the next number to examine */
        if (next_k > spf_limit) {
            idx_t new_limit = spf_limit * 2;
            idx_t *new_spf = NULL;
            build_spf(new_limit, &new_spf);
            free(spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        /* skip primes */
        while (next_k <= spf_limit && spf[next_k] == next_k)
            ++next_k;

        if (next_k > spf_limit)   /* safety guard; should never happen */
            continue;

        idx_t k = next_k++;        /* composite to process */

        /* find smallest and largest prime factors */
        idx_t temp = k;
        idx_t pmin = spf[temp];
        idx_t pmax = spf[temp];
        while (temp > 1) {
            idx_t p = spf[temp];
            if (p > pmax) pmax = p;
            while (temp % p == 0) temp /= p;
        }

        /* compute transformed product */
        seq_t prod = 1;
        bool used_pmin = false, used_pmax = false;  /* track one occurrence each */

        temp = k;
        while (temp > 1) {
            idx_t p = spf[temp];
            int mult = 0;
            while (temp % p == 0) {
                ++mult;
                temp /= p;
            }
            for (int i = 0; i < mult; ++i) {
                seq_t val = (seq_t)p;
                if (!used_pmin && p == pmin) {
                    val = (seq_t)(pmin + 1);
                    used_pmin = true;
                } else if (!used_pmax && p == pmax) {
                    val = (seq_t)(pmax - 2);
                    used_pmax = true;
                }
                prod *= val;
            }
        }

        /* skip if product is prime */
        if (!is_prime_big(prod)) {
            /* grow output array if needed */
            if (out_len >= out_cap) {
                size_t new_cap = out_cap ? out_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(output, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort */
                    free(spf);
                    free(output);
                    exit(1);
                }
                output = tmp;
                out_cap = new_cap;
            }
            output[out_len++] = prod;
        }
    }

    return output[n - 1];
}
