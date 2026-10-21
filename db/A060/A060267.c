/*
 * A060267  Difference between 2 closest primes surrounding 2n.
 * https://oeis.org/A060267
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_up(2n) – p_down(2n) where p_down is the largest prime
 *            less than 2n p_up is the smallest prime greater than 2n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;
static bool *sieve_prime = NULL;   /* sieve_prime[i] == true iff i is prime */
static uint32_t *prime_list = NULL;/* list of primes up to sieve_limit */
static size_t prime_count = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate sieve array */
    size_t sz = (size_t)limit + 1;
    bool *new_prime = (bool*)malloc(sz * sizeof(bool));
    if (!new_prime) abort();
    for (size_t i = 0; i < sz; ++i) new_prime[i] = true;
    if (sz > 0) new_prime[0] = false;
    if (sz > 1) new_prime[1] = false;

    /* Classic sieve */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (new_prime[p]) {
            for (uint64_t q = p * p; q <= limit; q += p)
                new_prime[q] = false;
        }
    }

    /* Build prime list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= (idx_t)limit; ++i)
        if (new_prime[i]) ++cnt;

    uint32_t *new_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_list) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= (idx_t)limit; ++i)
        if (new_prime[i]) new_list[idx++] = (uint32_t)i;

    /* Replace old data */
    free(sieve_prime);
    free(prime_list);
    sieve_prime = new_prime;
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

static idx_t prev_prime(idx_t n)
{
    if (n <= 2) return 0;   /* no prime below 2 */
    idx_t x = n - 1;
    while (x >= 2 && !sieve_prime[x]) --x;
    return x;
}

static idx_t next_prime(idx_t n)
{
    idx_t x = n + 1;
    while (x <= (idx_t)sieve_limit && !sieve_prime[x]) ++x;
    /* If we ran out of precomputed range, extend the sieve */
    if (x > (idx_t)sieve_limit) {
        ensure_primes((idx_t)(x + 100));   /* 100 is a safe margin */
        while (!sieve_prime[x]) ++x;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2, value unused below */

    idx_t m = 2 * n;

    /* Ensure we can find primes above 2n. 100 is ample for ranges ≤ 2·10⁵. */
    ensure_primes(m + 100);

    idx_t low  = prev_prime(m);
    idx_t high = next_prime(m);

    /* Safety: if something went wrong, return 0 */
    if (low == 0 || high == 0 || high <= low)
        return 0;

    return (seq_t)(high - low);
}
