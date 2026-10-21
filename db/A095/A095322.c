/*
 * A095322  Primes in whose binary expansion the number of 1 bits is > 4 +
 *          number of 0 bits.
 * https://oeis.org/A095322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth prime p such that
 *    popcount(p) > 4 + (bitlen(p) - popcount(p))
 * (i.e. the number of 1‑bits exceeds 4 plus the number of 0‑bits in the binary
 *  expansion of p).
 *
 * The function searches primes in increasing order, memoises results,
 * and returns the nth term.  It works for any seq_t that can be constructed
 * from a uint64_t and for any idx_t that is an unsigned integer type.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/_is_prime.h"

/* ---------- memoised storage of results ---------- */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static idx_t terms_len  = 0;

static void _grow_terms(size_t min)
{
    if (terms_cap >= min) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    if (new_cap < min) new_cap = min;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1 – harmless */

    /* ensure we have at least n terms */
    if ((idx_t)terms_len < n) {
        /* grow array for future use */
        _grow_terms((size_t)n);

        /* start searching from the next candidate after the last stored prime */
        uint64_t candidate = 2;
        if (terms_len > 0) {
            uint64_t last = (uint64_t)terms[terms_len - 1];
            candidate = last + 1;
            if ((candidate & 1ULL) == 0 && candidate != 2) candidate++;
        }

        while ((idx_t)terms_len < n) {
            if (_is_prime(candidate)) {
                unsigned ones  = __builtin_popcountll(candidate);
                unsigned bits  = 64 - __builtin_clzll(candidate);
                unsigned zeros = bits - ones;
                if (ones > 4 + zeros) {
                    terms[terms_len++] = (seq_t)candidate;
                }
            }
            candidate += (candidate == 2 ? 1 : 2);  /* skip even numbers */
        }
    }

    return terms[n - 1];
}
