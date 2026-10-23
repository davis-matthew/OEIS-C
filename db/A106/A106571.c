/*
 * A106571  Indices n of perfect squares n^2 which are not the difference of
 *          two primes.
 * https://oeis.org/A106571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106571: a(n) = n-th odd integer k such that k²+2 is composite
   (i.e., k² is not expressible as the difference of two primes).
   For even k the difference k² can always be written as p–q with primes p,q,
   so they are not part of the sequence. */
/* NOTE: prime test is performed on 64‑bit integers; for n up to ~10⁴
   this suffices (k²+2 ≤ 10⁸). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* 64‑bit deterministic Miller–Rabin primality test (deterministic for n<2⁶⁴). */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        __uint128_t x = 1, base = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * base) % n;
            base = (base * base) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Memoised list of terms (odd indices k for which k²+2 is composite). */
static seq_t *terms = NULL;
static idx_t term_count = 0;
static idx_t next_candidate = 1;   /* start checking from 1 */

/* Return the n‑th term of sequence A106571. */
seq_t a(idx_t n)
{
    while (term_count < n) {
        /* Process only odd candidates. */
        if (next_candidate & 1ULL) {
            uint64_t k = (uint64_t)next_candidate;
            uint64_t sq_plus_two = k * k + 2ULL;
            if (!is_prime_64(sq_plus_two)) {
                /* Store the odd integer k as a term. */
                seq_t *tmp = (seq_t *)realloc(terms, (term_count + 1) * sizeof(seq_t));
                if (!tmp) { abort(); }      /* allocation failure */
                terms = tmp;
                terms[term_count] = (seq_t)k;
                ++term_count;
            }
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
