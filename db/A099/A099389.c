/*
 * A099389  Subsequence of primes in sequence b(n) = 3*prime(n) - prime(n+1) -
 *          3 (A100021).
 * https://oeis.org/A099389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) is the nth prime that occurs in the subsequence of b(k) =
 *            3*prime(k) - prime(k+1) - 3 (sequence A100021). The function
 *            generates primes on demand and counts the primes among the b(k)
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;

/* Add a prime to the dynamic array */
static void add_prime(seq_t p) {
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        seq_t *new_list = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!new_list) abort();
        prime_list = new_list;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Ensure that prime_list contains at least primes up to index `up_to`
   (0‑based).  That is, prime_list[up_to] exists. */
static void ensure_primes_upto_index(idx_t up_to) {
    if (prime_cnt > up_to) return;
    seq_t candidate = prime_cnt == 0 ? 2 : prime_list[prime_cnt - 1] + 1;
    while (prime_cnt <= up_to) {
        bool is_prime = true;
        seq_t limit = 0;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            seq_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(candidate);
        candidate += 1;
    }
}

/* Primality test using the primes already computed */
static bool is_prime_seq(seq_t x) {
    if (x < 2) return false;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Main function computing A099389 */
seq_t a(idx_t n) {
    if (n <= 0) return 0;          /* offset is 1; behaviour below is undefined */
    idx_t count = 0;
    idx_t k = 1;                   /* 1‑based index for b(k) */
    while (count < n) {
        /* Need primes p_k and p_{k+1} */
        ensure_primes_upto_index(k);            /* primes up to p_k (index k-1) */
        ensure_primes_upto_index(k + 1);        /* primes up to p_{k+1} (index k) */

        seq_t pk   = prime_list[k - 1];
        seq_t pk1  = prime_list[k];             /* because index k+1 is k in 0‑based */
        seq_t b = 3 * pk - pk1 - 3;

        if (is_prime_seq(b)) {
            ++count;
            if (count == n) return b;
        }
        ++k;
    }
    return 0;  /* should never reach */
}
