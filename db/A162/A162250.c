/*
 * A162250  Values of the form prime(prime(i)) with a prime digital sum.
 * https://oeis.org/A162250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 368, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 992 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th prime of the form prime(prime(i))
 *            whose decimal digit sum is also prime.
 * Algorithm: generate primes on demand, test each prime(prime(i))
 *            for the digit‑sum condition, and memoize the valid terms.
 *            Each call to a(n) works independently of call order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_sum.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* ---------- Prime table ---------------------------------------- */

static idx_t  *prime_table = NULL;     /* prime_table[pos-1] = pos‑th prime */
static size_t prime_cap   = 0;
static size_t prime_len   = 0;
static idx_t  prime_next  = 2;        /* next candidate to test */

/* Ensure the prime table can hold 'new_cap' entries. */
static void grow_prime_table(size_t new_cap)
{
    if (prime_cap >= new_cap)
        return;
    idx_t *tmp =
        (idx_t *)realloc(prime_table, new_cap * sizeof(idx_t));
    if (!tmp && new_cap > 0)
        exit(EXIT_FAILURE);
    prime_table = tmp;
    prime_cap   = new_cap;
}

/* Make sure at least 'needed' primes are available in the table. */
static void ensure_primes_upto_index(idx_t needed)
{
    while (prime_len < needed) {
        /* Find next prime. */
        while (!is_prime_big((seq_t)prime_next))
            ++prime_next;
        grow_prime_table(prime_len + 1);
        prime_table[prime_len++] = prime_next;
        ++prime_next;
    }
}

/* ---------- Sequence memoization -------------------------------- */

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_i     = 1;      /* 1‑based index i for prime(i) */

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp && new_cap > 0)
            exit(EXIT_FAILURE);
        cache      = tmp;
        cache_cap  = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function --------------------------------------- */

seq_t a(idx_t n)
{
    /* Build up to the n‑th qualifying term. */
    while (cache_len < n) {
        /* prime(i) */
        ensure_primes_upto_index(next_i);
        idx_t prime_i = prime_table[next_i - 1];

        /* prime(prime(i)) */
        ensure_primes_upto_index(prime_i);
        seq_t candidate = prime_table[prime_i - 1];

        /* digit sum check */
        seq_t sum = digit_sum(candidate);
        if (is_prime_big(sum))
            cache_append(candidate);

        ++next_i;
    }
    return cache[n - 1];
}
