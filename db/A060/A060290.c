/*
 * A060290  Primes which are sums of twin Harshad numbers (includes overlaps).
 * https://oeis.org/A060290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 138, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_val, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060290: primes p such that (p-1)/2 and (p+1)/2 are Harshad numbers.
   The function computes the nth term for n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum_val.h"

/* Cached primes that satisfy the condition. */
static seq_t *prime_list = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;
static idx_t  next_k      = 0;   /* next k (≥1) to test */

/* Ensure the cache can hold at least 'needed' primes. */
static void grow_prime_list(void) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Add a new prime to the cache. */
static void add_prime(seq_t p) {
    grow_prime_list();
    prime_list[prime_len++] = p;
}

/* Generate primes until at least 'n' terms are found. */
static void ensure_n_primes(idx_t n) {
    while (prime_len < n) {
        ++next_k;                       /* start testing from 1 */
        idx_t k  = next_k;
        idx_t k1 = k + 1;

        idx_t ds_k  = digit_sum_val(k);
        if (ds_k == 0 || k % ds_k != 0) continue;

        idx_t ds_k1 = digit_sum_val(k1);
        if (ds_k1 == 0 || k1 % ds_k1 != 0) continue;

        seq_t sum = (seq_t)k + (seq_t)k1;   /* sum = 2k+1 */
        if (is_prime_big(sum))
            add_prime(sum);
    }
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 for safety */
    ensure_n_primes(n);
    return prime_list[n - 1];
}
