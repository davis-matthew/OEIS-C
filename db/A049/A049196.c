/*
 * A049196  Squarefree numbers whose Euler totient function is also
 *          squarefree.
 * https://oeis.org/A049196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 3805, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9665 (2026-11-03)
 * Helpers:   mem, mobius, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049196: squarefree numbers whose Euler totient is also squarefree.
   A number k is included iff mobius(k) ≠ 0 and mobius(phi(k)) ≠ 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Cached terms of the sequence. */
static seq_t *terms   = NULL;
static size_t terms_sz = 0;      /* number of stored terms                   */
static size_t terms_cap = 0;     /* current capacity                         */
static idx_t last_checked = 0;   /* last integer already evaluated           */

/* Ensure the term array can hold at least 'need' entries. */
static void ensure_terms_capacity(size_t need)
{
    if (terms_cap < need)
    {
        size_t new_cap = terms_cap ? terms_cap * 2 : 4;
        while (new_cap < need) new_cap <<= 1;
        terms = mem_resize(seq_t, terms, new_cap);
        terms_cap = new_cap;
    }
}

/* Test whether a given number satisfies the A049196 condition. */
static bool is_valid_a049196(idx_t k)
{
    /* k must be squarefree: mobius(k) ≠ 0 (i.e. ±1) */
    int mu_k = (int)mobius(k);
    if (mu_k == 0) return false;

    /* phi(k) must also be squarefree */
    seq_t phi_k = totient_seq((seq_t)k);
    int mu_phi = (int)mobius((idx_t)phi_k);
    return mu_phi != 0;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for accidental 0 index */

    /* Generate terms until we reach the requested index. */
    while (terms_sz < n)
    {
        ++last_checked;
        if (is_valid_a049196(last_checked))
        {
            ensure_terms_capacity(terms_sz + 1);
            terms[terms_sz++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
