/*
 * A202474  Coefficients of y^(n-1) in Product_{k=1..n} (1 + k*y + y^2) for n
 *          >= 1.
 * https://oeis.org/A202474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of y^(n-1) in ∏_{k=1..n} (1 + k*y + y^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation incrementally expands the product and caches the */
/* required coefficients so that multiple calls with arbitrary n are      */
/* answered in O(n) time per maximal n and O(1) for subsequent calls.     */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Dynamic array helpers (both C and C++) ---------- */
#ifdef __cplusplus
static seq_t* allocate_seq_array(size_t sz) {
    return new seq_t[sz];
}
static void free_seq_array(seq_t* p) {
    delete[] p;
}
#else
static seq_t* allocate_seq_array(size_t sz) {
    return (seq_t*)malloc(sz * sizeof(seq_t));
}
static void free_seq_array(seq_t* p) {
    free(p);
}
#endif

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0 as a guard. */
    if (n == 0) {
        return (seq_t)0;
    }

    static idx_t max_n = 0;          /* Largest n already computed              */
    static seq_t *coeff = NULL;      /* Coefficients of the current product      */
    static size_t coeff_cap = 0;     /* Current capacity of coeff array          */
    static seq_t *cache = NULL;      /* Cached a(k) values                       */
    static size_t cache_cap = 0;     /* Current capacity of cache array          */
    static seq_t *temp = NULL;       /* Temporary array for convolution          */
    static size_t temp_cap = 0;      /* Capacity of temp array                   */

    /* Allocate / resize coefficient and cache arrays to needed size. */
    if (n > max_n) {
        size_t need_deg = 2 * n;          /* Highest degree after expansion      */
        size_t need_cap = need_deg + 1;   /* Number of coefficient slots         */

        /* Expand coefficient array if required. */
        if (coeff_cap < need_cap) {
            seq_t *new_coeff = allocate_seq_array(need_cap);
            if (coeff) {
                /* Copy existing data to the larger array. */
                for (size_t i = 0; i < coeff_cap; ++i) {
                    new_coeff[i] = coeff[i];
                }
                free_seq_array(coeff);
            }
            coeff = new_coeff;
            coeff_cap = need_cap;
        }

        /* Expand cache array if required. */
        if (cache_cap < (size_t)(n + 1)) {
            seq_t *new_cache = allocate_seq_array((size_t)(n + 1));
            if (cache) {
                for (size_t i = 0; i < cache_cap; ++i) {
                    new_cache[i] = cache[i];
                }
                free_seq_array(cache);
            }
            cache = new_cache;
            cache_cap = (size_t)(n + 1);
        }

        /* Expand temporary array used for convolution. */
        if (temp_cap < need_cap) {
            seq_t *new_temp = allocate_seq_array(need_cap);
            if (temp) {
                free_seq_array(temp);
            }
            temp = new_temp;
            temp_cap = need_cap;
        }

        /* Initialize base case for n = 0 (product equals 1). */
        if (max_n == 0) {
            coeff[0] = (seq_t)1;
            for (size_t i = 1; i < need_cap; ++i) {
                coeff[i] = (seq_t)0;
            }
        }

        /* Incrementally extend the product from max_n+1 up to n. */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            size_t old_deg = 2 * (k - 1);
            size_t new_deg = 2 * k;

            /* Zero the temporary array up to the new degree. */
            for (size_t i = 0; i <= new_deg; ++i) {
                temp[i] = (seq_t)0;
            }

            seq_t k_val = (seq_t)k;

            /* Convolution with (1 + k*y + y^2). */
            for (size_t i = 0; i <= old_deg; ++i) {
                seq_t val = coeff[i];
                /* All operations are safe even if val == 0. */
                temp[i]     += val;                     /* * 1          */
                temp[i + 1] += val * k_val;             /* * k*y        */
                temp[i + 2] += val;                     /* * y^2        */
            }

            /* Copy the temporary coefficients back to the main array. */
            for (size_t i = 0; i <= new_deg; ++i) {
                coeff[i] = temp[i];
            }

            /* Store the required coefficient a(k) = coeff of y^(k-1). */
            cache[k] = coeff[k - 1];
        }

        max_n = n;
    }

    /* Return the cached value for the requested n. */
    return cache[n];
}
