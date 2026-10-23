/*
 * A122377  a(n) is the n-th term in periodic sequence repeating the divisors
 *          of n in increasing order.
 * https://oeis.org/A122377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 9859, u32 n <= 9859, u64 n <= 9859,
 *            u128 n <= 9859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = m-th divisor of n, where m = (n mod τ(n))
   with 0 mapped to τ(n).  It follows the OEIS definition
   that a(n) is the nth term in the periodic repetition of
   the divisors of n. */
#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on seq_t */
static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0; return 0 as sentinel */
        return 0;

    /* 1. Compute τ(n) – the number of positive divisors */
    idx_t tau = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i == n / i)
                ++tau;
            else
                tau += 2;
        }
    }

    /* 2. Find the position r in the periodic divisor list */
    idx_t r = n % tau;
    if (r == 0)
        r = tau;              /* map remainder 0 to τ(n) */

    /* 3. Enumerate all divisors into an array */
    seq_t *divs = (seq_t *)malloc(tau * sizeof(seq_t));
    if (!divs)
        return 0;              /* allocation failed */

    idx_t idx = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[idx++] = (seq_t)i;
            if (i != n / i)
                divs[idx++] = (seq_t)(n / i);
        }
    }

    /* 4. Sort the divisors in ascending order */
    qsort(divs, tau, sizeof(seq_t), compare_seq_t);

    /* 5. Return the r‑th divisor (1‑based) */
    seq_t result = divs[r - 1];
    free(divs);
    return result;
}
