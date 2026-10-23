/*
 * A040980  Primes p such that x^5 = 18 has a solution mod p.
 * https://oeis.org/A040980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040980
 * Primes p such that the congruence x^5 ≡ 18 (mod p) has a solution.
 * For primes p ≠ 1 (mod 5) the map x → x^5 is bijective in 𝔽_p,
 * so every element has a 5th root.  If p ≡ 1 (mod 5) then
 * 18 is a 5th power residue iff 18^((p-1)/5) ≡ 1 (mod p).  The
 * sequence consists of 2, 5 and all primes for which the above holds.
 * The implementation tests primes on demand and caches both the prime
 * list and the resulting sequence.  It relies only on the utilities
 * available in the build environment. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Append a value to a dynamic array. */
static void append_u64(uint64_t **arr, size_t *len, size_t *cap, uint64_t val) {
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[(*len)++] = val;
}
static void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[(*len)++] = val;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static uint64_t *prime_list    = NULL;
    static size_t   pl_len        = 0, pl_cap = 0;
    static seq_t   *seq           = NULL;
    static size_t   seq_len       = 0, seq_cap = 0;
    static uint64_t candidate     = 2;           /* next integer to test */

    while (seq_len < (size_t)n) {
        /* primality test by trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < pl_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            append_u64(&prime_list, &pl_len, &pl_cap, candidate);

            bool ok;
            if (candidate == 2 || candidate == 5) {
                ok = true;
            } else if (candidate % 5 != 1) {
                ok = true;
            } else {
                uint64_t exp = (candidate - 1) / 5;
                seq_t val = mod_pow((seq_t)18, exp, (seq_t)candidate);
                ok = (val == (seq_t)1);
            }

            if (ok) {
                append_seq(&seq, &seq_len, &seq_cap, (seq_t)candidate);
            }
        }
        ++candidate;
    }

    return seq[n - 1];
}
