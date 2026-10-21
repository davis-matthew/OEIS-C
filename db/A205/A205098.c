/*
 * A205098  Sum of proper divisors (A001065) of abundant numbers (A005101).
 * https://oeis.org/A205098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001065(A005101(n)) = A204822(n) - A005101(n)
 * Range:     u8 n <= 32, u16 n <= 6238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state for the divisor-sum sieve and the abundant-number list. */
static idx_t      current_limit = 0;   /* highest integer for which data are computed */
static uintmax_t *sigma         = NULL;/* sigma[i] = sum of divisors of i (as uintmax_t) */
static size_t     sigma_cap     = 0;    /* capacity of the sigma array */
static idx_t *    abundant      = NULL;/* list of abundant numbers (as idx_t) */
static size_t     abundant_cap = 0;    /* capacity of the abundant array */
static size_t     abundant_len = 0;    /* number of abundant numbers stored */

/* Recompute sigma[1..limit] and rebuild the abundant-number list. */
static void recompute(idx_t limit)
{
    /* free previous data if any */
    if (sigma) free((void *)sigma);
    if (abundant) free((void *)abundant);

    /* Allocate new arrays */
    sigma_cap = (size_t)limit + 1;
    sigma = (uintmax_t*)calloc(sigma_cap, sizeof(uintmax_t));
    if (!sigma) return;  /* allocation failed */

    /* Divisor-sum sieve (O(limit log limit)) */
    idx_t i, j;
    for (i = 1; i <= limit; ++i) {
        for (j = i; j <= limit; j += i) {
            sigma[j] += (uintmax_t)i;
        }
    }

    /* Build list of abundant numbers */
    abundant_cap = (size_t)limit;  /* initial capacity */
    abundant = (idx_t*)malloc(abundant_cap * sizeof(idx_t));
    if (!abundant) return;  /* allocation failed */
    abundant_len = 0;

    for (i = 1; i <= limit; ++i) {
        if (sigma[i] > (uintmax_t)i * 2) {     /* σ(i) > 2*i  →  i is abundant */
            if (abundant_len >= abundant_cap) {   /* grow capacity if needed */
                abundant_cap *= 2;
                abundant = (idx_t*)realloc(abundant, abundant_cap * sizeof(idx_t));
                if (!abundant) return;
            }
            abundant[abundant_len++] = i;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Ensure that we have at least n abundant numbers computed. */
    while (abundant_len < (size_t)n) {
        idx_t new_limit = current_limit == 0 ? 200000ULL : current_limit * 2ULL;
        recompute(new_limit);
        current_limit = new_limit;
    }

    idx_t  abundant_val   = abundant[n - 1];
    uintmax_t sigma_val   = sigma[abundant_val];
    uintmax_t proper_sum = sigma_val - (uintmax_t)abundant_val;

    return (seq_t)proper_sum;
}
