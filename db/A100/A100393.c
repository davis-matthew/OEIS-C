/*
 * A100393  Composite numbers k such that Gpf(k-1) < Gpf(k) > Gpf(k+1), where
 *          Gpf = A006530.
 * https://oeis.org/A100393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite k with Gpf(k‑1) < Gpf(k) > Gpf(k+1)
 * Range:     u8 n <= 34, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9620 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: build the sequence on demand using static caching. */
/* Requires the helper functions provided by the build environment. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *terms   = NULL;
    static idx_t terms_cap   = 0;
    static idx_t terms_size  = 0;
    static idx_t last_k = 2;     /* start searching from 2 */

    /* Generate terms until we have at least n of them */
    while (terms_size < n)
    {
        /* only consider composite numbers */
        if (!is_prime_u64((uint64_t)last_k))
        {
            seq_t g_prev = greatest_prime_factor((seq_t)(last_k - 1));
            seq_t g      = greatest_prime_factor((seq_t)           last_k);
            seq_t g_next = greatest_prime_factor((seq_t)(last_k + 1));

            if (g_prev < g && g > g_next)
            {
                push_seq(&terms, &terms_cap, &terms_size, (seq_t)last_k);
            }
        }
        ++last_k;
    }

    return terms[n - 1];
}
