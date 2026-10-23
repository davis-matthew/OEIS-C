/*
 * A156778  a(n) = n*A007504(n)/2 = n*(sum of first n primes)/2.
 * https://oeis.org/A156778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A007504(n)/2 = n*(sum of first n primes)/2
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static state for caching primes and cumulative sums */
static idx_t   current_limit = 0;
static char   *sieve_arr     = NULL;
static uint64_t *primes_arr  = NULL;
static seq_t  *cum_arr       = NULL;
static idx_t   primes_count  = 0;

/* compute primes up to at least nth prime and cumulative sums */
static void ensure_primes(idx_t n)
{
    if (n <= primes_count) return;            /* already have enough */

    /* estimate upper bound for nth prime (valid for n>=6) */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        double bound = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (idx_t)bound;
    }

    if (limit < current_limit) limit = current_limit; /* should not happen */

    /* allocate or reallocate sieve buffer */
    char *new_sieve = (char*)malloc((limit + 1) * sizeof(char));
    if (!new_sieve) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) new_sieve[i] = 0;

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!new_sieve[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                new_sieve[k] = 1;
        }
    }

    /* count primes */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_sieve[i]) ++cnt;

    /* reallocate primes and cumulative arrays if needed */
    uint64_t *tmp_primes = (uint64_t*)realloc(primes_arr, cnt * sizeof(uint64_t));
    seq_t *tmp_cum = (seq_t*)realloc(cum_arr, cnt * sizeof(seq_t));
    if (!tmp_primes || !tmp_cum) exit(EXIT_FAILURE);
    primes_arr = tmp_primes;
    cum_arr    = tmp_cum;

    /* fill arrays */
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_sieve[i]) {
            primes_arr[idx] = (uint64_t)i;
            if (idx == 0) {
                cum_arr[idx] = (seq_t)i;
            } else {
                cum_arr[idx] = cum_arr[idx - 1] + (seq_t)i;
            }
            ++idx;
        }
    }

    /* clean up old sieve */
    free(sieve_arr);
    sieve_arr = new_sieve;
    primes_count = cnt;
    current_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_primes(n);
    seq_t sum = cum_arr[n - 1];          /* sum of first n primes */
    seq_t res = ((seq_t)n * sum) / 2;
    return res;
}
