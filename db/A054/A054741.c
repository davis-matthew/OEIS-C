/*
 * A054741  Numbers m such that totient(m) < cototient(m).
 * https://oeis.org/A054741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th integer m such that totient(m) < cototient(m)
 * (i.e. φ(m) < m − φ(m), or m/φ(m) > 2).
 */
#include <stdlib.h>
#include "../utils/_phi.h"

/* dynamic array storing the sequence terms */
static seq_t   *terms = NULL;
static size_t  term_count = 0;   /* number of terms currently stored   */
static size_t  term_cap   = 0;   /* current capacity of the array       */
static idx_t   next_m     = 2;   /* next integer to test (start at 2)  */

/* ensure the terms array can hold at least new_cap elements */
static void grow_terms(size_t new_cap)
{
    if (term_cap >= new_cap) return;
    size_t cap = term_cap ? term_cap*2 : 64;
    while (cap < new_cap) cap *= 2;
    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    term_cap = cap;
}

/* a(idx_t n) – returns the n-th term of A054741 (offset 1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but avoid UB */

    /* extend the stored table until we have at least n terms */
    while (term_count < (size_t)n) {
        seq_t m_val   = (seq_t)next_m;
        seq_t phi_val = _phi(next_m);
        /* φ(m) < m – φ(m)  ⇔  φ(m) < m/2  */
        if (phi_val < m_val - phi_val) {
            grow_terms(term_count + 1);
            terms[term_count++] = m_val;
        }
        ++next_m;
    }

    return terms[n-1]; /* offset 1 → index n‑1 */
}
