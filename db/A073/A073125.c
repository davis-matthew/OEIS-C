/*
 * A073125  a(n) is the least prime p(s) such that prime(s+n) - prime(s) is
 *          divisible by n.
 * https://oeis.org/A073125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p_s such that p_{s+n} - p_s is divisible by n
 *            (p_k denotes the k‑th prime, indices are 1‑based)
 * Range:     u8 n <= 42, u16 n <= 760, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*----- Prime sieve helpers ----------------------------------------------------*/
static seq_t *prime_list = NULL;      /* array of primes in increasing order   */
static size_t prime_count = 0;        /* number of primes stored              */
static uint64_t sieve_limit = 0;      /* upper bound used for the sieve        */

/* Build the list of primes up to 'limit'.  All previous tables are freed. */
static void rebuild_primes(uint64_t limit)
{
    /* Allocate a boolean array via calloc (C++ requires an explicit cast). */
    char *is_prime = (char *)calloc((size_t)limit + 1U, 1U);
    if (!is_prime) return;               /* out of memory – abort silently */

    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes and allocate the result array. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    seq_t *new_list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!new_list) { free(is_prime); return; }

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[pos++] = (seq_t)i;

    free(is_prime);

    /* Replace old list with the new one. */
    free(prime_list);
    prime_list  = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure that at least 'needed' primes are available. If not, rebuild
   the sieve with a larger upper bound. */
static void ensure_primes_upto(size_t needed)
{
    if (prime_count >= needed) return;

    uint64_t new_limit = sieve_limit ? sieve_limit : 100U;
    /* Roughly double the limit until enough primes are found. */
    while (prime_count < needed) {
        if (new_limit < needed * 10) new_limit = needed * 10;
        rebuild_primes(new_limit);
        if (prime_count >= needed) break;
        new_limit *= 2;
    }
}

/*----- Main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Prime indices are 1‑based.  We need at least (n+1) primes because
       the search starts with s = 1 and may require prime(s+n). */
    size_t needed = (size_t)n + 1;
    ensure_primes_upto(needed);

    /* Convert n to seq_t once for modulo usage. */
    seq_t n_seq = (seq_t)n;

    for (size_t s = 1; ; ++s) {
        /* Ensure prime_count is at least s+n. */
        size_t required = s + n;
        if (prime_count < required) ensure_primes_upto(required);

        seq_t p_s = prime_list[s - 1];
        seq_t p_t = prime_list[s + n - 1];
        seq_t diff = p_t - p_s;

        if (diff % n_seq == 0)
            return p_s;
    }
}
