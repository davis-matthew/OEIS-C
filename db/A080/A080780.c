/*
 * A080780  a(1)=2; for n > 1, a(n) is the smallest integer greater than
 *          a(n-1) consistent with the condition "n is in the sequence if and
 *          only if a(n) is congruent to 0 (mod 6)".
 * https://oeis.org/A080780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A080780:
   a(1)=2; for n>1, a(n) is the smallest integer > a(n-1) such that
   (index n appears in the sequence)  <=>  (a(n) ≡ 0 (mod 6)).
   The implementation memoises values but allows calls with indices in any order.
*/

#include <stdlib.h>

static seq_t *seq      = NULL;   /* dynamic array of sequence values          */
static size_t seq_cap  = 0;      /* capacity of seq array                     */
static idx_t  seq_len  = 0;      /* number of values already computed          */

static unsigned char *used   = NULL;   /* boolean array, used[v] == 1 if v is a term */
static size_t        used_cap = 0;     /* capacity of used array                   */

static seq_t last = 0;

/* Ensure seq can hold at least n elements (1‑based). */
static void seq_ensure_cap(idx_t n)
{
    if ((size_t)n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

/* Ensure used can index at least v. */
static void used_ensure_cap(seq_t v)
{
    size_t idx = (size_t)v;
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    used = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!used) abort();
    /* initialise newly allocated entries to 0 */
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = 0;
    used_cap = new_cap;
}

/* Compute terms up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= seq_len) return;                 /* already done */

    /* initialise a(1) if first time */
    if (seq_len == 0) {
        seq_ensure_cap(1);
        seq[0] = (seq_t)2;
        seq_len = 1;
        last = (seq_t)2;
        used_ensure_cap(2);
        used[2] = 1;
    }

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        unsigned char inSet = 0;
        if ((size_t)i < used_cap) inSet = used[(size_t)i];

        seq_t cand = last + 1;
        while ( ((cand % (seq_t)6) == 0) != inSet ) ++cand;

        seq_ensure_cap(i);
        seq[i-1] = cand;
        seq_len = i;
        last = cand;

        used_ensure_cap(cand);
        used[(size_t)cand] = 1;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq[n-1];
}
