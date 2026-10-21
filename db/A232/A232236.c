/*
 * A232236  Primes that can be written in binary representation as
 *          concatenation of two primes: p and p+2. That is, primes
 *          representable as p * 2^L + p+2, where p and p+2 are primes, and L
 *          is the length of binary representation of p+2: L = A070939(p+2).
 * https://oeis.org/A232236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = p * 2^L + (p + 2), where p and p + 2 are twin primes and
 *            L = bitlen(p + 2). This function generates those terms on demand
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big, is_prime_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

/* ---------- Cached values ----------
 * terms are stored zero‑based: seq_cache[0] = a(1).  */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;  /* capacity of seq_cache   */
static size_t cache_len = 0;  /* number of terms stored */

/* ---------- Twin prime search ----------
 * occ_n   – the current odd candidate for p in a twin pair.  */
static idx_t occ_n = 3;

/* Append a new term to the cache, growing the array if needed. */
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* malloc failure: terminate */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; lower indices are irrelevant */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Skip non‑prime candidates. */
        if (!is_prime_small((seq_t)occ_n)) {
            occ_n += 2;
            continue;
        }
        if (!is_prime_small((seq_t)(occ_n + 2))) {
            occ_n += 2;
            continue;
        }

        /* Bit length of the right part (p + 2). */
        idx_t L = bitlen_seq_t((seq_t)(occ_n + 2));

        /* Concatenate the binary representations. */
        seq_t val = ((seq_t)occ_n) * pow2(L) + (occ_n + 2);

        /* Store the term if it is prime. */
        if (is_prime_big(val))
            append_term(val);

        occ_n += 2;
    }

    return seq_cache[n - 1];
}
