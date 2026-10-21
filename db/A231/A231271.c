/*
 * A231271  Numbers k such that k and k+d are both odious (A000069) or both
 *          evil (A001969) for every divisor d of k.
 * https://oeis.org/A231271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Brute‑force search of candidates satisfying the divisor
   parity property.  For each integer k we test all divisors d of k and
   require that parity(k) == parity(k+d), where parity means the parity
   of the binary popcount (odious ⇔ parity = 1, evil ⇔ parity = 0). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Parity of a value of type seq_t (works for arbitrary‑precision ints). */
static bool parity_seq_t(seq_t x)
{
    bool p = false;
    while (x != 0) {
        p ^= (bool)(x & 1);
        x >>= 1;
    }
    return p;
}

/* Test whether a 64‑bit integer k satisfies the property. */
static bool is_valid_k(uint64_t k)
{
    bool pk = parity_seq_t((seq_t)k);
    for (uint64_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) {
            /* divisor d = i */
            seq_t sum = (seq_t)k + (seq_t)i;
            if (parity_seq_t(sum) != pk)
                return false;
            /* divisor d = k/i (if distinct) */
            uint64_t d = k / i;
            if (d != i) {
                sum = (seq_t)k + (seq_t)d;
                if (parity_seq_t(sum) != pk)
                    return false;
            }
        }
    }
    return true;
}

/* Storage for already found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a new term to the cache, expanding capacity if needed. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* Function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined behaviour, offset is 1 */

    /* Generate terms until we have the n‑th one. */
    uint64_t cand = 1;
    if (terms_len > 0) cand = (uint64_t)terms[terms_len - 1] + 1;

    while (terms_len < (size_t)n) {
        if (is_valid_k(cand))
            append_term((seq_t)cand);
        ++cand;
    }
    return terms[n - 1];
}
