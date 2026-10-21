/*
 * A071127  Number of times there is a prime p < n such that 2n+p is prime.
 * https://oeis.org/A071127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ p < n | p prime and 2n+p prime }
 * Range:     u8 n <= 3989, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static idx_t prime_sieve_limit = 0;
static uint8_t *prime_flags = NULL;

/* extend the sieve to cover all numbers up to |limit| */
static void extend_sieve(idx_t limit)
{
    if (limit <= prime_sieve_limit)
        return;

    /* create a flag array for primality */
    uint8_t *flags = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!flags) return;                       /* out‑of‑memory guard */
    for (idx_t i = 0; i <= limit; ++i) flags[i] = 1;
    flags[0] = flags[1] = 0;

    /* simple Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (flags[i])
            for (idx_t j = i * i; j <= limit; j += i)
                flags[j] = 0;

    /* collect the primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (flags[i]) ++cnt;
    uint64_t *list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (flags[i]) list[pos++] = (uint64_t)i;

    /* replace global data */
    free(prime_list);
    prime_list = list;
    prime_list_cnt = cnt;

    free(prime_flags);
    prime_flags = flags;
    prime_sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* offset starts at 1 */

    /* ensure we have primes up to 3*n (the largest candidate 2n+p) */
    idx_t needed_limit = 3 * n;
    if (needed_limit > prime_sieve_limit)
        extend_sieve(needed_limit);

    seq_t count = 0;
    for (size_t i = 0; i < prime_list_cnt && prime_list[i] < n; ++i) {
        idx_t p = prime_list[i];
        idx_t cand = 2 * n + p;
        if (cand <= prime_sieve_limit && prime_flags[cand])
            ++count;
    }
    return count;
}
