/*
 * A154302  Primes of the form (1+2+...+m)/87 = A000217(m)/87.
 * https://oeis.org/A154302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   a(n) = the n‑th prime of the form T(m)/87, where T(m)=m(m+1)/2
 *            that is p = m(m+1)/174. Only m ≡ 0,29,57,86 (mod 87) yield an
 *            integer
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cache of found primes */
static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t last_m_checked = 0;

/* append a prime to the cache, growing it if necessary */
static void add_prime_to_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed – terminate */
        prime_cache = tmp;
        cache_cap = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* return the n‑th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0 but safe */

    while (cache_size < (size_t)n) {
        last_m_checked++;                     /* next candidate m */
        uint64_t m = last_m_checked;
        uint64_t r = m % 87;

        /* only m with residues 0,29,57,86 give an integer result */
        if (r != 0 && r != 29 && r != 57 && r != 86) continue;

        seq_t mseq = (seq_t)m;
        seq_t p = (mseq * (mseq + 1)) / (seq_t)174;  /* p = m(m+1)/174 */

        if (is_prime_big(p)) add_prime_to_cache(p);
    }

    return prime_cache[n - 1];
}
