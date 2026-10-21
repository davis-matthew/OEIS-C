/*
 * A226228  Numbers m such that all lengths of runs of 1's in the binary
 *          representation of m are prime numbers.
 * https://oeis.org/A226228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (n‑th) integer whose binary representation contains
 *            runs of 1's only of prime length (OEIS A226228)
 * Range:     u8 n <= 47, u16 n <= 2979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the sequence lazily.  It keeps a static
   cache of already found terms and extends it until the cache
   contains at least n elements.  The cache reuse means that
   calls can come in any order.  The sequence is built by
   testing consecutive integers, starting from 1, and
   keeping those that satisfy the run‑length condition.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"        /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */
#include "../utils/is_prime_small.h" /* bool is_prime_small(idx_t n) */

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* next integer to test (as idx_t to avoid overflow) */
static idx_t next_test = 1;

/* extend the cache until it contains at least 'need' terms */
static void ensure_cache(size_t need)
{
    if (cache_size >= need)
        return;

    while (cache_size < need) {
        /* stop if we overflow the seq_t type */
        if ((seq_t)next_test != next_test)
            break;   /* no more values can be represented in seq_t */

        seq_t m = (seq_t)next_test++;
        bool ok = true;
        seq_t t = m;

        /* examine runs of 1's in the binary representation of m */
        while (t) {
            /* skip trailing zeros */
            while ((t & 1) == 0) {
                t >>= 1;
                if (!t)
                    break;
            }
            if (!t)
                break;

            /* count the length of a run of 1's */
            unsigned run_len = 0;
            while ((t & 1) == 1) {
                run_len++;
                t >>= 1;
            }

            /* if the run length is not prime, discard this number */
            if (!is_prime_small((idx_t)run_len)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = m;
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;
    ensure_cache((size_t)n); /* generate terms up to n */
    return cache[n - 1];
}
