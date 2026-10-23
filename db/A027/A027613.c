/*
 * A027613  Degrees of polynomials involved in analysis of a binary-tree
 *          valued Markov chain depending on one parameter; each polynomial
 *          has a unique root in [ 0,1 ], which is a lower bound for the
 *          transition from positive recurrence to null recurrence.
 * https://oeis.org/A027613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n primes minus 1 if n ≥ 4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *cumsum = NULL;          /* cumulative sum: cumsum[i] = sum of first i primes */
static idx_t   cumsum_size = 0;       /* number of primes currently stored */

/* Generate primes up to the required count and compute cumulative sums */
static void generate_sieve_and_sums(idx_t target_n)
{
    /* estimate upper bound for the target_n-th prime (PNT upper bound) */
    size_t limit;
    if (target_n < 6) {
        limit = 15;
    } else {
        double dn = (double)target_n;
        double estimate = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (size_t)estimate + 1;
    }

    /* simple sieve of Eratosthenes */
    unsigned char *is_prime = (unsigned char *)malloc(limit + 1);
    if (!is_prime) return;          /* allocation failed, abort silently */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes up to the limit */
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;

    /* allocate array for primes */
    uint64_t *pr = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!pr) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < primes_cnt; ++i) {
        if (is_prime[i]) pr[idx++] = (uint64_t)i;
    }

    /* compute cumulative sums */
    if (cumsum) free(cumsum);
    seq_t *new_cumsum = (seq_t *)malloc((primes_cnt + 1) * sizeof(seq_t));
    if (!new_cumsum) { free(pr); free(is_prime); return; }

    new_cumsum[0] = (seq_t)0;
    for (size_t i = 1; i <= primes_cnt; ++i) {
        new_cumsum[i] = new_cumsum[i - 1] + (seq_t)pr[i - 1];
    }

    /* clean up and store results */
    free(pr);
    free(is_prime);

    cumsum      = new_cumsum;
    cumsum_size = primes_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of defined range, return 0 */
    if (n > cumsum_size) generate_sieve_and_sums(n);

    seq_t result = cumsum[(size_t)n];
    if (n >= 4) result -= (seq_t)1;        /* subtract 1 for n ≥ 4 */
    return result;
}
