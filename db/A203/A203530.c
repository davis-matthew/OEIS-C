/*
 * A203530  a(n) = Product_{1 <= i < j <= n} (c(i) + c(j)); c = A002808 =
 *          composite numbers.
 * https://oeis.org/A203530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{1<=i<j<=n} (c(i)+c(j)), with c the sequence of
 *            composite numbers
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  For n = 1 there are no pairs, so the product is 1. */
    if (n <= 1)
        return (seq_t)1;

    /* ------------------------------------------------------------------ */
    /* Find the first n composite numbers by sieving.  The sieve limit is  */
    /* increased progressively until enough composites are found.        */
    /* ------------------------------------------------------------------ */
    idx_t limit = (n * 3) + 10;                     /* heuristic start */
    while (1) {
        /* Allocate the sieve array. */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime)
            return (seq_t)0;  /* allocation failure */

        /* Initialise. */
        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = (i >= 2);
        for (idx_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;

        /* Count composites to see if the current limit is sufficient. */
        idx_t comp_count = 0;
        for (idx_t i = 4; i <= limit; ++i)
            if (!is_prime[i])
                ++comp_count;

        /* If we have enough composites, collect them. */
        if (comp_count >= n) {
            seq_t *comps = (seq_t *)malloc(n * sizeof(seq_t));
            if (!comps) {
                free(is_prime);
                return (seq_t)0;    /* allocation failure */
            }
            idx_t k = 0;
            for (idx_t i = 4; i <= limit && k < n; ++i)
                if (!is_prime[i])
                    comps[k++] = (seq_t)i;
            free(is_prime);

            /* ------------------------------------------------------------------ */
            /* Compute the product over all pairs i < j.  Use the sequence‑type    */
            /* multiplication directly; seq_t is an unsigned integer type or     */
            /* an arbitrary‑precision integer class, so the operator* works.      */
            /* ------------------------------------------------------------------ */
            seq_t result = (seq_t)1;
            for (idx_t i = 0; i < n - 1; ++i) {
                for (idx_t j = i + 1; j < n; ++j) {
                    result *= (comps[i] + comps[j]);
                }
            }

            free(comps);
            return result;
        }

        /* Not enough composites – double the limit and try again. */
        free(is_prime);
        limit <<= 1;
    }
}
