/*
 * A059257  Primes p such that x^47 = 2 has no solution mod p.
 * https://oeis.org/A059257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that x^47 ≡ 2 (mod p) has no
 *            solution. Equivalently, p ≡ 1 (mod 94) and 2^((p‑1)/47) ≠ 1 (mod
 *            p)
 * Range:     u8 none, u16 n <= 138, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *terms = NULL;        /* stored primes that satisfy the condition */
static size_t terms_sz = 0;        /* number of stored terms     */
static size_t terms_cap = 0;       /* allocated capacity of `terms` */

static uint64_t last_checked = 1;  /* prime examined last; next_prime starts after this */

static void grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* memory exhaustion: abort */
    terms = tmp;
    terms_cap = new_cap;
}

/* Generate primes until we have at least `n` valid terms in `terms`. */
static void assure_terms(size_t n)
{
    while (terms_sz < n) {
        uint64_t p = next_prime(last_checked);
        last_checked = p;

        /* Test only primes ≡ 1 (mod 94). */
        if (p % 94ULL != 1ULL) continue;

        uint64_t exp = (p - 1ULL) / 47ULL;
        seq_t v = mod_pow((seq_t)2, exp, (seq_t)p);

        /* Add p if 2 is NOT a 47th power modulo p. */
        if (v != (seq_t)1) {
            grow_terms(terms_sz + 1);
            terms[terms_sz++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0 */
    size_t need = (size_t)n;
    assure_terms(need);
    return terms[n - 1];
}
