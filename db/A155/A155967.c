/*
 * A155967  Binary transpose primes. Integers of k^2 bits which, when written
 *          row by row as a square matrix and then read column by column, are
 *          primes once transformed.
 * https://oeis.org/A155967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A155967:
   Binary transpose primes.  For a prime p, let len = bitlen(p).
   If len is a perfect square k², write p row‑by‑row into a k×k
   matrix, then read it column‑by‑column.  If the resulting number
   q is also prime, p is a term of the sequence.
   a(1) = 11, a(2) = 13, a(3) = 257, … 
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* integer square root for idx_t values (no overflow) */
static idx_t int_sqrt_idx(idx_t n)
{
    idx_t r = 0;
    for (int i = (sizeof(idx_t) * 8 >> 1) - 1; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t t   = r + bit;
        if (t <= n / t) r = t;
    }
    return r;
}

/* test whether a prime satisfies the transpose‑prime property */
static bool test_prime(seq_t p)
{
    /* number of bits in p */
    idx_t len = (idx_t)bitlen_seq_t(p);
    if (len == 0) return false;                      /* safety */

    /* k² = len ? */
    idx_t k = int_sqrt_idx(len);
    if (k * k != len) return false;

    /* column‑by‑column read */
    seq_t q = 0;
    for (idx_t c = 0; c < k; ++c) {
        for (idx_t r = 0; r < k; ++r) {
            idx_t bit_index = (idx_t)(len - 1) - (r * k + c);
            seq_t bit = (p >> bit_index) & 1;
            q = (q << 1) | bit;
        }
    }
    return is_prime_big(q);
}

/* cache of sequence terms */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;

/* current candidate for the next prime to test */
static seq_t prime_candidate = 0;
static bool  cache_initialized = false;

/* grow cache array */
static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = seq_alloc_array(new_cap);
    for (size_t i = 0; i < seq_cache_len; ++i)
        new_cache[i] = seq_cache[i];
    seq_free_array(seq_cache);
    seq_cache = new_cache;
    seq_cache_cap = new_cap;
}

/* generate terms until we have at least n of them */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (seq_cache_len >= (size_t)n) return;

    if (!cache_initialized) {
        prime_candidate = 2;
        cache_initialized = true;
    }

    while (seq_cache_len < (size_t)n) {
        /* find next prime */
        while (!is_prime_big(prime_candidate)) prime_candidate++;

        if (test_prime(prime_candidate)) {
            if (seq_cache_len >= seq_cache_cap) {
                size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
                grow_cache(new_cap);
            }
            seq_cache[seq_cache_len++] = prime_candidate;
        }
        /* move to next candidate (skip even numbers after 2) */
        prime_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, but silences warnings */
    ensure_cache(n);
    return seq_cache[n - 1];
}
