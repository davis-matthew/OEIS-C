/*
 * A086733  Convolution of sigma(n) with phi(n).
 * https://oeis.org/A086733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} sigma(k) * phi(n-k+1)
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *sigma_arr = NULL;
static uint64_t *phi_arr   = NULL;
static uint64_t *spf_arr   = NULL;
static size_t  spf_size   = 0;
static size_t  max_precomp = 0;

/* Recompute sigma and phi up to 'limit' using smallest prime factor sieve */
static void recompute(size_t limit)
{
    /* Expand spf array */
    if (limit + 1 > spf_size) {
        uint64_t *old = spf_arr;
        spf_arr = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
        spf_size = limit + 1;
        free(old);
        /* linear sieve for smallest prime factor */
        for (size_t i = 2; i <= limit; ++i) {
            if (spf_arr[i] == 0) {
                spf_arr[i] = i;
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i) {
                        if (spf_arr[j] == 0)
                            spf_arr[j] = i;
                    }
                }
            }
        }
    }

    /* Resize sigma and phi arrays */
    sigma_arr = (uint64_t *)realloc(sigma_arr, (limit + 1) * sizeof(uint64_t));
    phi_arr   = (uint64_t *)realloc(phi_arr,   (limit + 1) * sizeof(uint64_t));

    sigma_arr[1] = 1;
    phi_arr[1]   = 1;
    for (size_t n = 2; n <= limit; ++n) {
        uint64_t m = n;
        uint64_t sigma_val = 1;
        uint64_t phi_val   = 1;
        while (m > 1) {
            uint64_t p = spf_arr[m];
            uint64_t pow_p = 1;
            while (m % p == 0) {
                m /= p;
                pow_p *= p;
            }
            /* sum of powers of p: (p^{exp+1}-1)/(p-1) */
            uint64_t sum_p = (pow_p * p - 1) / (p - 1);
            sigma_val *= sum_p;
            /* phi contribution: p^{exp-1} * (p-1) */
            uint64_t phi_part = (pow_p / p) * (p - 1);
            phi_val *= phi_part;
        }
        sigma_arr[n] = sigma_val;
        phi_arr[n]   = phi_val;
    }
    max_precomp = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if ((size_t)n > max_precomp)
        recompute((size_t)n);

    uint64_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += sigma_arr[(size_t)k] * phi_arr[(size_t)(n - k + 1)];

    return (seq_t)sum;
}
