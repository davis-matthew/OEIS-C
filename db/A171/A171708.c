/*
 * A171708  Numbers k such that the k-th prime of the form 6*k-1 and the k-th
 *          prime of the form 6*k+1 are a pair of twin primes.
 * https://oeis.org/A171708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A171708:  numbers k such that the k‑th prime of the
 *  form 6*k-1 and the k‑th prime of the form 6*k+1 are a pair of twin
 *  primes.  This implementation does not use a closed formula but
 *  constructs the sequence by iterating over primes in ascending order,
 *  maintaining the two class‑specific prime lists and recording the
 *  indices for which the twins occur.  The procedure is memoised so
 *  that any number of calls to a(n) is fast. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint64_t *prime_all = NULL;  /* all primes found so far */
    static size_t prime_len = 0, prime_cap = 0;

    static uint64_t *p1 = NULL;   /* primes ≡1 (mod 6) */
    static size_t  p1_len = 0, p1_cap = 0;

    static uint64_t *p5 = NULL;   /* primes ≡5 (mod 6) */
    static size_t  p5_len = 0, p5_cap = 0;

    static uint64_t *matches = NULL; /* sequence of k */
    static size_t  match_len = 0, match_cap = 0;

    static uint64_t next_candidate = 5;  /* next odd number to test */

    /* grow auxiliary arrays */
    if (prime_cap == 0) {
        prime_cap = 8; prime_all = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (p1_cap == 0) {
        p1_cap = 8; p1 = (uint64_t *)malloc(p1_cap * sizeof(uint64_t));
    }
    if (p5_cap == 0) {
        p5_cap = 8; p5 = (uint64_t *)malloc(p5_cap * sizeof(uint64_t));
    }
    if (match_cap == 0) {
        match_cap = 8; matches = (uint64_t *)malloc(match_cap * sizeof(uint64_t));
    }

    /* generate sequence until we have at least n matches */
    while (match_len < n) {
        /* primality test by trial division using the previously found primes */
        int is_prime = 1;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_all[i];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            /* store in list of all primes */
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                prime_all = (uint64_t *)realloc(prime_all, prime_cap * sizeof(uint64_t));
            }
            prime_all[prime_len++] = next_candidate;

            /* handle the two residue classes separately */
            uint64_t mod6 = next_candidate % 6;
            if (mod6 == 5) {
                if (p5_len == p5_cap) {
                    p5_cap *= 2;
                    p5 = (uint64_t *)realloc(p5, p5_cap * sizeof(uint64_t));
                }
                p5[p5_len++] = next_candidate;
                size_t k = p5_len - 1;
                if (p1_len > k) {
                    uint64_t diff = p1[k] > p5[k] ? p1[k] - p5[k] : p5[k] - p1[k];
                    if (diff == 2) {
                        if (match_len == match_cap) {
                            match_cap *= 2;
                            matches = (uint64_t *)realloc(matches, match_cap * sizeof(uint64_t));
                        }
                        matches[match_len++] = k + 1;
                    }
                }
            } else if (mod6 == 1) {
                if (p1_len == p1_cap) {
                    p1_cap *= 2;
                    p1 = (uint64_t *)realloc(p1, p1_cap * sizeof(uint64_t));
                }
                p1[p1_len++] = next_candidate;
                size_t k = p1_len - 1;
                if (p5_len > k) {
                    uint64_t diff = p1[k] > p5[k] ? p1[k] - p5[k] : p5[k] - p1[k];
                    if (diff == 2) {
                        if (match_len == match_cap) {
                            match_cap *= 2;
                            matches = (uint64_t *)realloc(matches, match_cap * sizeof(uint64_t));
                        }
                        matches[match_len++] = k + 1;
                    }
                }
            }
        }

        /* advance to next odd number */
        next_candidate += 2;
    }

    return (seq_t)matches[n - 1];
}
