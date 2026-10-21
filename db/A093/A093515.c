/*
 * A093515  Numbers k such that either k or k-1 is a prime.
 * https://oeis.org/A093515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th positive integer k such that k or k-1 is prime
 *            (offset 1)
 * Range:     u8 n <= 107, u16 n <= 9905, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* storage for generated primes and sequence values */
static idx_t *prime_list = NULL;
static size_t  prime_count = 0, prime_cap = 0;

static seq_t *seq_vals = NULL;
static size_t  seq_len = 0, seq_cap = 0;

/* ensure prime array has room for at least one more element */
static void ensure_prime_cap(void)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        idx_t *tmp = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap = new_cap;
    }
}

/* ensure sequence array has room for at least one more element */
static void ensure_seq_cap(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap = new_cap;
    }
}

/* simple primality test using the primes we have already generated */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > x / p) break;      /* p > sqrt(x) */
        if (x % p == 0) return false;
    }
    return true;
}

/* a(n) = nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for out‑of‑bounds calls */

    while (seq_len < n) {
        /* find the next prime to process */
        idx_t cand = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
        while (!is_prime_idx(cand)) ++cand;

        /* store the new prime */
        ensure_prime_cap();
        prime_list[prime_count++] = cand;

        /* output the prime itself if it hasn’t appeared already */
        if (seq_len == 0 || seq_vals[seq_len - 1] != (seq_t)cand) {
            ensure_seq_cap();
            seq_vals[seq_len++] = (seq_t)cand;
        }

        /* output the prime+1 if it hasn’t appeared already */
        idx_t nxt = cand + 1;
        if (seq_len == 0 || seq_vals[seq_len - 1] != (seq_t)nxt) {
            ensure_seq_cap();
            seq_vals[seq_len++] = (seq_t)nxt;
        }
    }

    return seq_vals[n - 1];
}
