/*
 * A107166  Primes of the form 2x^2 + 29y^2.
 * https://oeis.org/A107166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime of the form 2 x² + 29 y² (primes ≡
 *            2,15,21,29,31,37,39,47,55,61,69,77,79,85,95,101,
 *            119,127,133,135,143,157,159,189,191,205,213,215,221,229 (mod
 *            232))
 * Range:     u8 n <= 12, u16 n <= 1675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------- static storage ------------------- */
static uint64_t *prime_list   = NULL;   /* all primes found so far   */
static size_t   prime_cap    = 0;
static size_t   prime_len    = 0;

static uint64_t *subset_list = NULL;   /* primes of the desired form */
static size_t   subset_cap   = 0;
static size_t   subset_len   = 0;

static uint64_t next_candidate = 2;     /* next integer to test */

/* residues modulo 232 that correspond to the quadratic form */
static const uint32_t residues[] = {
    2, 15, 21, 29, 31, 37, 39, 47, 55, 61,
    69, 77, 79, 85, 95, 101, 119, 127, 133,
    135, 143, 157, 159, 189, 191, 205, 213,
    215, 221, 229
};
static const size_t residues_len = sizeof(residues)/sizeof(residues[0]);

static void ensure_prime_cap(void) {
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap = new_cap;
    }
}

static void ensure_subset_cap(void) {
    if (subset_len >= subset_cap) {
        size_t new_cap = subset_cap ? subset_cap << 1 : 16;
        uint64_t *tmp = (uint64_t *)realloc(subset_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        subset_list = tmp;
        subset_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour below is irrelevant */

    /* Ensure we have at least n terms in subset_list */
    while (subset_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* primality test */
        bool prime = true;
        if (cand > 2 && (cand & 1) == 0) {
            prime = false;
        } else {
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t p = prime_list[i];
                unsigned __int128 p2 = (unsigned __int128)p * p;
                if (p2 > cand) break;
                if (cand % p == 0) { prime = false; break; }
            }
        }

        if (prime) {
            /* keep prime in the general list */
            ensure_prime_cap();
            prime_list[prime_len++] = cand;

            /* test whether candidate fits the quadratic form */
            uint64_t mod = cand % 232;
            for (size_t i = 0; i < residues_len; ++i) {
                if (mod == (uint64_t)residues[i]) {
                    ensure_subset_cap();
                    subset_list[subset_len++] = cand;
                    break;
                }
            }
        }
        ++next_candidate;
    }

    return (seq_t)subset_list[n - 1];
}
