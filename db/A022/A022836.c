/*
 * A022836  a(n) = c(1)*p(0) + ... + c(n)*p(n-1), where c(i) = 1 if a(i-1) <=
 *          p(i-1) and c(i) = -1 if a(i-1) > p(i-1) (with p(0) = 1 and p(i) a
 *          prime for i >= 1).
 * https://oeis.org/A022836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+p(n) if a(n-1)<=p(n) else a(n-1)-p(n)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Dynamic array helpers ------------------------------------------------*/
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    void *new_ptr = realloc(*ptr, new_cap * elem_size);
    if (!new_ptr) exit(1);
    *ptr = new_ptr;
    *cap = new_cap;
}

/*--- Prime generation ------------------------------------------------------*/
static uint64_t *primes = NULL;        /* 0‑based array: primes[0]=2, primes[1]=3, ... */
static size_t   primes_cap = 0;
static idx_t    primes_cnt = 0;        /* number of primes stored, zero‑based */
static uint64_t next_prime_candidate = 2; /* next odd number to test */

/* Make sure primes[0 .. target] exist (target is zero‑based index).      */
static void ensure_primes_upto(idx_t target)
{
    if (primes_cnt > target) return;
    ensure_capacity(target + 1, &primes_cap, (void **)&primes, sizeof(uint64_t));

    while (primes_cnt <= target) {
        /* Test current candidate for primality using known primes.  */
        bool is_prime = true;
        for (idx_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p * p > next_prime_candidate) break;
            if (next_prime_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            primes[primes_cnt++] = next_prime_candidate;
        }

        /* Advance candidate: skip even numbers after 2. */
        if (next_prime_candidate == 2)
            next_prime_candidate = 3;
        else
            next_prime_candidate += 2;
    }
}

/* Return the value of the prime with index idx (p(0)=1, p(1)=2, ...).      */
static inline seq_t prime_val(idx_t idx)
{
    if (idx == 0) return (seq_t)1;           /* special case for p(0)=1   */
    ensure_primes_upto(idx - 1);              /* need primes[idx-1] = p(idx) */
    return (seq_t)primes[idx - 1];
}

/*--- Sequence cache --------------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;                /* highest computed index + 1 */

/* Compute a(n) if not already computed.                                   */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    if (cache_len == 0) {                    /* initialise a(0) */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) continue;                /* already set */
        seq_t prev = cache[i - 1];
        seq_t p    = prime_val(i);
        cache[i] = (prev <= p) ? prev + p : prev - p;
    }
    cache_len = n + 1;
}

/*--- Public API -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
