/*
 * A080064  Numbers k such that k == 1 modulo (lpf(k)+1), where lpf(k) is the
 *          least prime dividing k (A020639).
 * https://oeis.org/A080064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A080064: the sequence of integers k such that
   k ≡ 1 (mod (lpf(k)+1)), where lpf(k) is the least prime factor
   of k (with lpf(1)=1). The function generates terms on demand
   by checking each integer in order and stores them in a static
   array for reuse. */

#include <stdint.h>
#include <stdlib.h>

/* forward declarations for helper functions */
static uint64_t least_prime_factor(uint64_t n);
static void add_prime(uint64_t p);
static uint64_t next_prime_candidate(void);

/* static storage for primes discovered during factorisation */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

static void add_prime(uint64_t p)
{
    if (prime_count == prime_capacity) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_capacity = new_cap;
    }
    prime_list[prime_count++] = p;
}

/* least prime factor of n (n>=2). For n==1 returns 1. */
static uint64_t least_prime_factor(uint64_t n)
{
    if (n <= 1) return 1;

    /* trial division by already known primes */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return p;
    }

    /* if reached here, n is prime (or its smallest factor is > sqrt(n)) */
    if (prime_count == 0 || n > prime_list[prime_count-1])
        add_prime(n);
    return n;
}

/* static storage for generated sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_capacity = 0;
static uint64_t cur_k = 1;  /* next integer to test */

/* helper to grow sequence cache */
static void grow_seq_cache(void)
{
    if (seq_len == seq_capacity) {
        size_t new_cap = seq_capacity ? seq_capacity * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though not used */

    /* generate terms until we have the nth one */
    while (seq_len < (size_t)n) {
        uint64_t lpf = least_prime_factor(cur_k);
        seq_t modval = (seq_t)cur_k % ((seq_t)lpf + 1);
        if (modval == 1) {
            grow_seq_cache();
            seq_cache[seq_len++] = (seq_t)cur_k;
        }
        ++cur_k;
    }

    return seq_cache[(size_t)n - 1];
}
