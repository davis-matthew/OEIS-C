/*
 * A219505  Numbers that are arithmetic mean of 5 successive primes.
 * https://oeis.org/A219505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic mean of 5 successive primes, stored as the
 *            smallest integer m such that m = (p_k + p_{k+1} + p_{k+2} +
 *            p_{k+3} + p_{k+4}) / 5 for some k. a(1) = 79, a(2) = 281, …
 * Range:     u8 none, u16 n <= 340, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates primes and caches the means
   so that a(n) can be queried in any order efficiently. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- Prime generator (trial division) ------------------------ */
static uint64_t *prime_arr = NULL;   /* dynamic array of primes (uint64_t) */
static size_t    prime_cap = 0;      /* capacity of prime_arr          */
static size_t    prime_cnt = 0;      /* number of primes generated     */

/* Ensure at least 'need' primes are present (index up to need-1). */
static void ensure_primes(size_t need)
{
    if (prime_arr == NULL) {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_arr[prime_cnt++] = 2;
        prime_arr[prime_cnt++] = 3;
    }
    while (prime_cnt <= need) {
        uint64_t cand = prime_arr[prime_cnt-1] + 2;
        for (;;) {
            bool prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_arr[i];
                if (p * p > cand) break;
                if (cand % p == 0) { prime = false; break; }
            }
            if (prime) break;
            cand += 2;
        }
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap * 2;
            if (new_cap == 0) new_cap = 16;
            uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
            prime_arr = tmp;
            prime_cap = new_cap;
        }
        prime_arr[prime_cnt++] = cand;
    }
}

/* --------- Mean cache ------------------------------------------------ */
static seq_t *mean_arr = NULL;      /* dynamic array of means (seq_t)   */
static size_t mean_cap = 0;         /* capacity of mean_arr             */
static size_t mean_len = 0;         /* number of means stored           */
static idx_t  processed_start = 0;  /* index of first prime in current  window */

/* Resizing helper for seq_t array that works in C and C++ */
static void *grow_seq_arr(void *old, size_t old_cap, size_t new_cap)
{
    if (new_cap <= old_cap) return old;
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) tmp[i] = ((seq_t *)old)[i];
    delete [] (seq_t *)old;
    return tmp;
#else
    seq_t *tmp = (seq_t *)realloc(old, new_cap * sizeof(seq_t));
    return tmp;
#endif
}

/* ensure_means(i) guarantees mean_arr[0..i-1] are filled (1‑based) */
static void extend_means(idx_t i)
{
    for ( ; mean_len < (size_t)i; ++processed_start ) {
        /* we need primes up to processed_start+4 */
        ensure_primes(processed_start + 4);

        seq_t sum = 0;
        for (size_t j = 0; j < 5; ++j)
            sum += (seq_t)prime_arr[processed_start + j];

        if (sum % (seq_t)5 == 0) {
            seq_t mean = sum / (seq_t)5;
            if (mean_len == mean_cap) {
                size_t new_cap = mean_cap ? mean_cap * 2 : 16;
                mean_arr = (seq_t *)grow_seq_arr(mean_arr, mean_cap, new_cap);
                mean_cap = new_cap;
            }
            mean_arr[mean_len++] = mean;
        }
    }
}

/* ---------------------------------------------------- */
/* Public sequence function                              */
/* ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, harmless */
    extend_means(n);
    return mean_arr[n-1];
}
