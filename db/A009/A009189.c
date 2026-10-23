/*
 * A009189  Expansion of e.g.f.: exp(cos(x)*x).
 * https://oeis.org/A009189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 25, i128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 1
   a(n) = sum_{k=0..floor((n-1)/2)} (-1)^k * binomial(n-1,2*k) * (2*k+1) * a(n-2*k-1)
*/

#include <stdlib.h>
#include <stddef.h>
#if defined(__cplusplus)
# include <new>
#endif
#include "../utils/binomial.h"

/* ---- dynamic array helpers that work in C and C++ ---- */
#if defined(__cplusplus)
static seq_t *alloc_seq_array(size_t sz) { return ::new seq_t[sz]; }
static seq_t *realloc_seq_array(seq_t *old, size_t oldsz, size_t newsz) {
    seq_t *newp = ::new seq_t[newsz];
    for (size_t i = 0; i < oldsz; ++i) newp[i] = old[i];
    delete[] old;
    return newp;
}
#else
static seq_t *alloc_seq_array(size_t sz) { return (seq_t*)malloc(sz*sizeof(seq_t)); }
static seq_t *realloc_seq_array(seq_t *old, size_t oldsz, size_t newsz) {
    (void)oldsz;
    return (seq_t*)realloc(old, newsz*sizeof(seq_t));
}
#endif

/* ---- cache for computed values ---- */
static seq_t *memo = NULL;
static size_t memo_cap = 0;
static idx_t memo_last = (idx_t)-1;   /* last computed index */

/* Resize memo so it holds at least indices 0 … n */
static void ensure_sequence_capacity(idx_t n) {
    if ((idx_t)memo_cap > n) return;
    size_t newcap = memo_cap>0? memo_cap: 2;
    while ((idx_t)newcap <= n) newcap *= 2;
    memo = realloc_seq_array(memo, memo_cap, newcap);
    memo_cap = newcap;
}

/* External function required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    if (memo == NULL) {                   /* first call: initialise cache */
        memo_cap = 2;
        memo = alloc_seq_array(memo_cap);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo_last = 1;
    }

    if (n <= memo_last) return memo[n];   /* already computed */

    ensure_sequence_capacity(n);

    for (idx_t i = memo_last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t kmax = (i - 1) / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t two_k = 2 * k;
            seq_t comb = binomial(i - 1, two_k);
            seq_t term = comb * (seq_t)(2 * k + 1);
            term = term * memo[i - two_k - 1];
            if (k & 1) sum -= term; /* (-1)^k */
            else       sum += term;
        }
        memo[i] = sum;
    }

    memo_last = n;
    return memo[n];
}
