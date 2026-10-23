/*
 * A133677  Integers k such that prime(k)*(2*prime(k)-1)/3 is an integer.
 * https://oeis.org/A133677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that the k‑th prime P_k
 *            satisfies P_k mod 3 ≠ 1 (i.e. P_k mod 3 is 0 or 2). Equivalent
 *            to: k such that P_k ≡ 0 or 2 (mod 3)
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime generation by index ---------- */
static uint64_t *prime_list = NULL;      /* stored primes */
static size_t prime_cap = 0, prime_len = 0;
static uint64_t next_candidate = 2;      /* next number to test for primality */

/* Ensure prime_list has at least idx primes.  Caller must provide idx ≥ 1. */
static void
ensure_prime_index(size_t idx)
{
    if (prime_len >= idx) return;
    while (prime_len < idx) {
        /* test next_candidate for primality */
        uint64_t cand = next_candidate;
        size_t j = 0;
        int is_prime = 1;
        for (j = 0; j < prime_len; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* enlarge array if necessary */
            if (prime_len >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_len++] = cand;
        }
        ++next_candidate;
    }
}

/* Return the k-th prime (1‑based).  k ≥ 1. */
static uint64_t
prime_at_idx(size_t k)
{
    ensure_prime_index(k);
    return prime_list[k - 1];
}

/* ---------- Cache for sequence a(n) ---------- */
static seq_t *a_cache = NULL;
static size_t a_cap = 0, a_len = 0;
static size_t processed_k = 0;   /* last k processed */

/* Ensure a_cache has at least n entries; compute as necessary. */
static void
ensure_a_len(size_t n)
{
    if (a_len >= n) return;
    while (a_len < n) {
        ++processed_k;
        uint64_t p = prime_at_idx(processed_k);
        if (p % 3 != 1) {    /* includes 0 and 2 mod 3 */
            if (a_len >= a_cap) {
                size_t new_cap = a_cap ? a_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                a_cache = tmp;
                a_cap = new_cap;
            }
            a_cache[a_len++] = (seq_t)processed_k;
        }
    }
}

/* ---------- Public sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;    /* undefined before offset, but just return 0 */
    ensure_a_len((size_t)n);
    return a_cache[(size_t)n - 1];
}
