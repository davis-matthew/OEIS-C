/*
 * A239428  Number of ordered ways to write n = k + m with 0 < k <= m such
 *          that pi(2*k) - pi(k) and pi(2*m) - pi(m) are both prime, where
 *          pi(x) denotes the number of primes not exceeding x.
 * https://oeis.org/A239428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with k+m=n , 0<k<=m, * and
 *            pi(2k)-pi(k) and pi(2m)-pi(m) are both prime. * * This
 *            implementation builds a sieve up to 2*n and uses it to evaluate
 *            * the required differences. The function is memoized with a
 *            static * sieve that grows as needed
 * Range:     u8 n <= 2242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static bool *sieve_composite = NULL;
static idx_t *sieve_pi = NULL;
static idx_t sieve_limit = 0;

/* Build or extend a sieve up to 'limit'.  The array 'sieve_pi[x]'
 * contains π(x).  The boolean array marks composites. */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;
    if (sieve_composite)
        free(sieve_composite);
    if (sieve_pi)
        free(sieve_pi);
    sieve_limit = limit;
    sieve_composite = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    sieve_pi = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    idx_t count = 0;
    sieve_pi[0] = 0;
    if (limit >= 1)
        sieve_pi[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve_composite[i]) {
            ++count;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve_composite[j] = true;
            }
        }
        sieve_pi[i] = count;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    /* need π(2*k) and π(2*m) for k,m<=n, so build up to 2*n */
    idx_t needed_limit;
    /* avoid overflow: cap at maximum representable value */
    if (n > ((idx_t)~0ULL) / 2 + 1)
        needed_limit = ((idx_t)~0ULL);
    else
        needed_limit = n * 2;
    build_sieve(needed_limit);

    seq_t result = 0;
    for (idx_t k = 1; k <= n / 2; ++k) {
        idx_t m = n - k;
        idx_t dk = sieve_pi[2 * k] - sieve_pi[k];
        if (!is_prime_small((seq_t)dk))
            continue;
        idx_t dm = sieve_pi[2 * m] - sieve_pi[m];
        if (!is_prime_small((seq_t)dm))
            continue;
        ++result;
    }
    return result;
}
