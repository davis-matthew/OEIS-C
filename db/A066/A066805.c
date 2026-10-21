/*
 * A066805  a(n) is the least k such that n + Sum_{i=1..k} A018252(i) is
 *          prime; or 0 if none exists.
 * https://oeis.org/A066805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n + sum_{i=1..k} A018252(i) is prime; 0
 *            if none exists
 * Range:     u8 n <= 75, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively generate nonprime sequence and test primes. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static data retained between calls */
    static seq_t *cum_prefix = NULL;   /* cum_prefix[0] = 0, cum_prefix[k] = sum of first k nonprimes */
    static size_t cum_cap = 0;
    static idx_t nonprime_cnt = 0; /* number of nonprimes stored, cum_prefix[nonprime_cnt] valid */
    static idx_t next_candidate = 1;

    /* initialise static storage on first call */
    if (cum_prefix == NULL) {
        cum_cap = 2;                      /* need at least indices 0 and 1 */
        cum_prefix = (seq_t *)malloc(cum_cap * sizeof(seq_t));
        cum_prefix[0] = (seq_t)0;
        nonprime_cnt = 0;
        next_candidate = 1;
    }

    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        /* ensure the sum of the first k nonprimes is known */
        while (nonprime_cnt < k) {
            /* find next nonprime integer */
            while (is_prime_big((seq_t)next_candidate))
                ++next_candidate;

            /* grow the array if necessary */
            if (cum_cap <= nonprime_cnt + 1)
                grow_seq_array(&cum_prefix, &cum_cap, nonprime_cnt + 2);

            /* store cumulative sum */
            cum_prefix[nonprime_cnt + 1] = cum_prefix[nonprime_cnt] + (seq_t)next_candidate;
            ++nonprime_cnt;
            ++next_candidate;
        }

        seq_t candidate = (seq_t)n + cum_prefix[k];
        if (is_prime_big(candidate)) {
            result = (seq_t)k;
            break;
        }
        /* per the definition a solution exists for all n >= 1 */
    }

    return result;
}
