/*
 * A097997  Primes of the form prime(n) + prime(n+1) + prime(n+4).
 * https://oeis.org/A097997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) + p(n+1) + p(n+4) where the result is a prime,
 *            1-indexed (a(1) is the first term). The function builds primes
 *            lazily using trial division and keeps a list of the discovered
 *            terms
 * Range:     u8 n <= 12, u16 n <= 631, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *prime_list   = NULL;   /* primes generated so far      */
static size_t   prime_len    = 0;       /* number of primes stored      */
static size_t   prime_cap    = 0;       /* current capacity of prime_list */

static seq_t  *term_list     = NULL;    /* terms of the sequence        */
static size_t  term_len      = 0;       /* number of terms stored       */
static size_t  term_cap      = 0;       /* capacity of term_list        */

static idx_t   next_index    = 1;       /* next k to evaluate           */

/* ---------- simple primality test for uint64_t ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

/* ---------- extend the prime list up to a given count ---------- */
static void ensure_primes_up_to(size_t target)
{
    if (prime_len >= target) return;

    /* start from either 2 or one more than the last prime */
    uint64_t candidate = prime_len ? (uint64_t)prime_list[prime_len - 1] + 1 : 2;

    while (prime_len < target) {
        /* test candidate for primality */
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 64;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_len++] = (uint32_t)candidate;
        }
        ++candidate;
    }
}

/* ---------- extend the term list up to a given count ---------- */
static void ensure_terms_up_to(size_t target)
{
    if (term_len >= target) return;

    while (term_len < target) {
        /* ensure we have primes up to next_index+4 */
        ensure_primes_up_to(next_index + 4);

        /* indices are 1‑based; convert to 0‑based for the array */
        uint64_t sum =
            (uint64_t)prime_list[next_index - 1]   /* p(n)   */
          + (uint64_t)prime_list[next_index]     /* p(n+1) */
          + (uint64_t)prime_list[next_index + 3];/* p(n+4) */

        if (is_prime_uint64(sum)) {
            seq_t val = (seq_t)sum;
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(term_list, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                term_list = tmp;
                term_cap = new_cap;
            }
            term_list[term_len++] = val;
        }
        ++next_index;
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for offset‑0 callers   */
    ensure_terms_up_to((size_t)n);
    return term_list[(size_t)n - 1];
}
