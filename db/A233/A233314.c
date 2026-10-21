/*
 * A233314  Primes p such that p+6 and p+12 are composite.
 * https://oeis.org/A233314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A233314 by searching primes and checking that p+6 and p+12 are composite */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static uint64_t _last_checked = 1;      /* last examined TINK prime candidate */
static seq_t   *_terms = NULL;          /* cache of found terms */
static size_t  _len = 0;                /* number of stored terms */
static size_t  _cap = 0;                /* allocated capacity */

/* Return the n-th term of A233314 (1‑based index).  The function is
   cache‑aware and works correctly when called in any order.  It uses
   an auxiliary routine seq_realloc for memory management so that it
   compiles with both fixed‑size and arbitrary‑precision seq_t. */
seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (_len < (size_t)n) {
        /* Get next prime after the last one examined. */
        uint64_t p = next_prime(_last_checked);
        _last_checked = p;

        /* Check the two required conditions. */
        bool ok = !is_prime_big((seq_t)(p + 6ULL)) &&
                  !is_prime_big((seq_t)(p + 12ULL));

        if (ok) {
            /* Reallocate the array if necessary. */
            if (_len >= _cap) {
                size_t new_cap = _cap ? _cap * 2 : 64;
                seq_t *tmp = seq_realloc(_terms, _cap, new_cap);
                if (!tmp) {
                    /* Out of memory – return a zero value. */
                    return (seq_t)0;
                }
                _terms = tmp;
                _cap   = new_cap;
            }
            /* Store the term. */
            _terms[_len++] = (seq_t)p;
        }
    }
    return _terms[n - 1];
}
