/*
 * A099742  Least number B such that (A001359(n)-B^2)^2+B is also the lesser
 *          of larger twin primes, or 0 if no such B exists.
 * https://oeis.org/A099742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least B such that
 *   (A001359(n) - B^2)^2 + B is the lower member of a twin prime pair
 *   higher than A001359(n).  Returns 0 if no such B is found
 *   (conjecturally never occurs for n ≥ 1).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *lower_twin = NULL;      /* array of lower twin primes */
static size_t lower_cnt  = 0;         /* current number of stored twin primes */
static size_t lower_cap  = 0;         /* current capacity of the array   */
static seq_t next_candidate = 3;      /* next odd integer to test for twin prime */

static void ensure_lower_twin_primes(idx_t n)
{
    if (lower_cnt >= (size_t)n)
        return;

    /* initialise or grow the twin array as needed */
    if (lower_cap == 0) {
        lower_cap = 32;
        lower_twin = (seq_t *)malloc(lower_cap * sizeof(seq_t));
    }

    while (lower_cnt < (size_t)n) {
        seq_t cand = next_candidate;

        /* test for twin prime at cand and cand+2 */
        if (isPrimeGeneric(cand) && isPrimeGeneric(cand + 2U)) {
            if (lower_cnt == lower_cap) {
                lower_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(lower_twin, lower_cap * sizeof(seq_t));
                if (!tmp) {
                    free(lower_twin);
                    exit(EXIT_FAILURE);
                }
                lower_twin = tmp;
            }
            lower_twin[lower_cnt++] = cand;
        }
        next_candidate = cand + 2U;
    }
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;

    /* obtain the n-th lower twin prime */
    ensure_lower_twin_primes(n);
    seq_t p = lower_twin[n - 1U];

    /* search for the minimal B with the required property */
    for (idx_t B = 1U; ; ++B) {
        seq_t Bsq  = (seq_t)B * (seq_t)B;      /* B^2  */
        seq_t diff = p > Bsq ? p - Bsq : Bsq - p; /* |p - B^2| */
        seq_t diffSq = diff * diff;            /* (|p - B^2|)^2 */
        seq_t x = diffSq + (seq_t)B;           /* candidate lower prime */

        /* reject if the candidate is not greater than the source prime */
        if (x <= p)
            continue;

        if (isPrimeGeneric(x) && isPrimeGeneric(x + 2U))
            return (seq_t)B;
    }
}
