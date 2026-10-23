/*
 * A194874  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-sqrt(6).
 * https://oeis.org/A194874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at position pos in the permutation of 1..r,
 *            where r satisfies r*(r+1)/2 ≥ n, sorted by decreasing fractional
 *            parts of k*(-√6)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Constants for scaled sqrt(6).  SCALE = 10^18. */
static const uint64_t SCALE   = 1000000000000000000ULL;
static const uint64_t SQRT6   = 2449489742783178000ULL; /* floor(√6 * SCALE) */

/* Helper structure for permutation sorting. */
typedef struct {
    uint64_t key; /* (k * SQRT6) mod SCALE, used for ordering */
    uint64_t idx; /* original index k (1‑based) */
} perm_t;

/* Comparator: descending by key. */
static int cmp_perm(const void *p1, const void *p2)
{
    const perm_t *a = (const perm_t *)p1;
    const perm_t *b = (const perm_t *)p2;
    if (a->key > b->key) return -1;
    if (a->key < b->key) return  1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless */

    /* Find the row r that contains position n. */
    idx_t r  = 1;
    idx_t cum = 1;                 /* cumulative counts up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position within the r‑th row (1‑based). */
    idx_t prev = (r - 1) * r / 2;   /* terms before row r */
    idx_t pos  = n - prev;          /* desired position in this row */

    /* Build permutation array for row r. */
    perm_t *perm = (perm_t *)malloc(r * sizeof(perm_t));
    if (!perm) return 0;           /* allocation failed */

    for (idx_t k = 1; k <= r; ++k) {
        __uint128_t prod = (__uint128_t)k * SQRT6;
        perm[k - 1].key = (uint64_t)(prod % SCALE);
        perm[k - 1].idx = k;
    }

    /* Sort indices by descending key, i.e., ascending fractional part of
       k*(-√6). */
    qsort(perm, r, sizeof(perm_t), cmp_perm);

    seq_t result = (seq_t)perm[pos - 1].idx;
    free(perm);
    return result;
}
