/*
 * A109365  a(-1)=a(0)=1 and recursively a(n) = prime(n)*(a(n-1)+a(n-2)).
 * https://oeis.org/A109365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(-1)=a(0)=1, a(n) = prime(n)*(a(n-1)+a(n-2)) (n>=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 – a(0) is the first term listed above. */
/* Recurrence uses the (n+1)-st prime: prime(1)=2, prime(2)=3, ... */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime generation ------------------------------------------------------ */
/* primes[1] = 2, primes[2] = 3, ... */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;   /* number of primes stored */

static void extend_primes_up_to(size_t need)
{
    if (prime_cnt >= need)
        return;

    if (prime_cnt == 0) { /* seed with first prime */
        size_t init_cap = 128;
        prime_list = (uint64_t *)realloc(prime_list, init_cap * sizeof(uint64_t));
        prime_cap = init_cap;
        prime_cnt = 1;
        prime_list[1] = 2;
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt] + 1;
    if (candidate % 2 == 0 && candidate != 2)
        candidate++;

    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 1; i <= prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt + 1 >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                prime_cap = new_cap;
            }
            prime_list[++prime_cnt] = candidate;
        }
        candidate += 2;   /* only odd numbers beyond 2 */
    }
}

/* Sequence storage ------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_highest = 0;  /* highest index stored */

/* Ensure capacity for indices 0..need . */
static void ensure_seq_capacity(idx_t need)
{
    if ((size_t)need + 1 <= seq_cap)
        return;

    size_t new_cap = (need + 1) * 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Initialize cache with a(0)=1, a(1)=4 */
static void init_seq_cache(void)
{
    if (seq_cache)
        return;

    seq_cap = 8;
    seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)4;
    seq_highest = 1;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    init_seq_cache();

    /* Ensure we have computed up to n. */
    if (n > seq_highest) {
        /* Need primes up to index n (prime(n)). */
        extend_primes_up_to((size_t)n);

        /* Extend cache. */
        ensure_seq_capacity(n);

        /* Compute missing terms. */
        for (idx_t i = seq_highest + 1; i <= n; ++i) {
            seq_t sum = seq_cache[i - 1] + seq_cache[i - 2];
            seq_t p   = (seq_t)prime_list[i];  /* i-th prime */
            seq_cache[i] = sum * p;
        }
        seq_highest = n;
    }

    return seq_cache[n];
}
