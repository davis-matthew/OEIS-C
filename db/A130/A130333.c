/*
 * A130333  Numbers k >= 1 for which k!!/Sum_{i=1..k} k is an integer.
 * https://oeis.org/A130333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: k is in the sequence iff k!! is divisible by k(k+1)/2
   (i.e. by the sum 1+2+…+k).  The function computes the list of such
   k by successive search and memoises the results so that any n≥0 can be
   answered in essentially O(1) after the first few values are generated. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef __cplusplus
/* In C we can use plain malloc/free for simple integer types. */
#else
/* In C++ we need placement new to construct big integer objects in raw
   malloc'ed memory. */
#include <new>
#endif

/* Helpers for modular multiplication. For small seq_t types this is trivial;
   for big integer types the built‑in operators are used. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return mod == 0 ? (seq_t)0 : (a * b) % mod;
}

/* Memoised storage of the sequence. */
static seq_t*   terms      = NULL;   /* array of k values that satisfy the property */
static size_t   term_cnt   = 0;       /* number of elements stored so far        */
static size_t   term_cap   = 0;       /* allocated capacity                      */
static seq_t   last_k      = 0;       /* last checked k                          */

/* Grow the array of terms, constructing/destroying objects as needed
   (C++ only). */
static void grow_terms(size_t new_cap)
{
    seq_t* new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();

    if (terms) {
        /* Copy existing elements to new array. */
        for (size_t i = 0; i < term_cnt; ++i) {
#ifdef __cplusplus
            new(&new_ptr[i]) seq_t(terms[i]);  /* placement new */
#else
            new_ptr[i] = terms[i];
#endif
        }
        /* Destroy old objects (C++ only). */
#ifdef __cplusplus
        for (size_t i = 0; i < term_cnt; ++i) {
            terms[i].~seq_t();
        }
#endif
        free(terms);
    }
    terms    = new_ptr;
    term_cap = new_cap;
}

/* Generate terms until we have at least (n+1) entries. */
static void ensure_terms(idx_t n)
{
    while (term_cnt <= (size_t)n) {
        /* Advance to the next candidate k. */
        last_k++;

        /* Compute sum = k(k+1)/2 using seq_t arithmetic. */
        seq_t sum = ((seq_t)last_k * ((seq_t)last_k + 1)) / 2;

        /* Build k!! modulo sum. */
        seq_t prod = 1;
        seq_t start = (last_k & 1) ? (seq_t)1 : (seq_t)2;   /* first factor of correct parity */

        for (seq_t f = start; f <= last_k; f += 2) {
            prod = mul_mod(prod, f, sum);
            if (prod == 0) break;      /* already divisible, stop early */
        }

        if (prod == 0) {               /* k satisfies the condition */
            if (term_cnt == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                grow_terms(new_cap);
            }
            terms[term_cnt++] = last_k;
        }
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[(size_t)n];
}
