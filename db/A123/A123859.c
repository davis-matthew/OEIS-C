/*
 * A123859  a(1)=1. a(n) = a(n-1) + (sum of the terms, from among terms a(1)
 *          through a(n-1), which divide sum{k=1 to n-1} a(k)).
 * https://oeis.org/A123859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>1, a(n)=a(n-1)+∑{a(k): 1≤k<n and a(k) divides
 *            Σ_{i=1}^{n-1} a(i)}
 * Range:     u8 n <= 11, u16 n <= 81, u32 n <= 1128, u64 n <= 1128,
 *            u128 n <= 1128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *memo = NULL;   /* 1-based array: memo[1] is a(1) */
static idx_t last   = 0;     /* highest computed index */
static seq_t  sumPrev = 0;   /* Σ_{i=1}^{last} a(i)   */

/* ensure memo has at least size elements (1-based). returns new capacity */
static size_t ensure_capacity(idx_t size) {
    if (size <= last) return size;
    size_t cap = size * 2; /* grow factor */
    seq_t *tmp = (seq_t *)realloc(memo, cap * sizeof(seq_t));
    if (!tmp) abort();
    memo = tmp;
    return cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but safe */
    if (n <= last) return memo[n];
    /* grow array to at least n */
    ensure_capacity(n);
    if (last == 0) {
        /* initialise a(1) */
        memo[1] = (seq_t)1;
        sumPrev = (seq_t)1;
        last = 1;
    }
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t divSum = 0;
        /* find all previous terms that divide sumPrev */
        for (idx_t j = 1; j <= last; ++j) {
            if (sumPrev % memo[j] == (seq_t)0) {
                divSum += memo[j];
            }
        }
        memo[k] = memo[last] + divSum;
        sumPrev += memo[k];
        last = k;
    }
    return memo[n];
}
