/*
 * A158974  a(n) is the number of numbers k <= n such that not all proper
 *          divisors of k are divisors of n.
 * https://oeis.org/A158974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #k (1≤k≤n) for which some proper divisor d of k does not
 *            divide n
 * Range:     u8 n <= 325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_non_dividing_proper_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_non_dividing_proper_divisor.h"

/*------------------------------------------------------------------*/
/*  Smallest prime factor sieve (up to a dynamic limit).            */
/*------------------------------------------------------------------*/
static idx_t cur_spf_limit = 0;
static idx_t *spf_arr      = NULL;

/* Allocate / enlarge the SPF table to include all numbers ≤ limit. */
static void
ensure_spf(idx_t limit)
{
    if (limit <= cur_spf_limit)
        return;

    free(spf_arr);
    spf_arr = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf_arr[j] == 0)
                        spf_arr[j] = i;
            }
        }
    }
    cur_spf_limit = limit;
}

/*------------------------------------------------------------------*/
/*  Check whether k has a proper divisor that is not a divisor of n.*/

/*------------------------------------------------------------------*/
/*  Public sequence function.                                       */
/*------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_spf(n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (has_non_dividing_proper_divisor(k, n, spf_arr))
            ++count;
    return count;
}
