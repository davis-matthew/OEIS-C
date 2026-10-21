/*
 * A074997  Numbers which are sandwiched between two numbers having the same
 *          unordered canonical form.
 * https://oeis.org/A074997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that the multiset of exponents in *
 *            the prime factorizations of (m−1) and (m+1) are equal. This
 *            means * that m is sandwiched between two numbers with the same
 *            unordered * canonical form
 * Range:     u8 n <= 40, u16 n <= 7254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Test whether two integers have the same unordered canonical form
 * (i.e. the same multiset of exponents in their prime factorisations).*/
static bool same_canonical_form(uint64_t a, uint64_t b)
{
    /* These arrays are large enough for any 64–bit integer. */
    uint64_t primesA[64], expsA[64];
    size_t cntA = 0;
    uint64_t primesB[64], expsB[64];
    size_t cntB = 0;

    factor_u64(a, primesA, expsA, &cntA);
    factor_u64(b, primesB, expsB, &cntB);

    if (cntA != cntB)
        return false;

    /* sort the exponent arrays (simple bubble sort – fine for tiny sizes). */
    for (size_t i = 0; i + 1 < cntA; ++i)
        for (size_t j = i + 1; j < cntA; ++j)
            if (expsA[i] > expsA[j]) {
                uint64_t t = expsA[i];
                expsA[i] = expsA[j];
                expsA[j] = t;
            }

    for (size_t i = 0; i + 1 < cntB; ++i)
        for (size_t j = i + 1; j < cntB; ++j)
            if (expsB[i] > expsB[j]) {
                uint64_t t = expsB[i];
                expsB[i] = expsB[j];
                expsB[j] = t;
            }

    /* compare the sorted exponent lists */
    for (size_t i = 0; i < cntA; ++i)
        if (expsA[i] != expsB[i])
            return false;

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static uint64_t next     = 3;   /* first candidate will be 4 */

    while (cache_len < n) {
        ++next;                                 /* candidate m */
        if (same_canonical_form(next - 1, next + 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *new_ptr = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                if (!new_ptr)
                    return 0; /* out of memory – unlikely for n≈10000 */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next;
        }
    }
    return cache[n - 1];
}
