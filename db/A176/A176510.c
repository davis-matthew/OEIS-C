/*
 * A176510  Triangle, read by rows, defined by T(n, m) = b(n) - b(m) - b(n-m)
 *          + 1, where b(n) = b(n-1) + b(n-2) - b(n-3) + b(n-5), with b(0) =
 *          0, b(1) = 1, b(2) = 1, b(3) = 2, b(4) = 2.
 * https://oeis.org/A176510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r is the row such that r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 and c = n - r(r+1)/2. T(r,c) = b(r) - b(c) - b(r-c)
 *            + 1, with b defined by b(0)=0, b(1)=1, b(2)=1, b(3)=2, b(4)=2
 *            and for n≥5: b(n)=b(n-1)+b(n-2)-b(n-3)+b(n-5)
 * Range:     u8 n <= 174, u16 n <= 564, u32 n <= 2020, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the sequence b */
static seq_t *b_cache = NULL;
static idx_t b_cap  = 0;   /* allocated capacity */
static idx_t b_len  = 0;   /* number of initialized entries */

/* Ensure that b_cache contains values up to index n */
static void ensure_b(idx_t n)
{
    /* Initial allocation and seed values */
    if (b_cache == NULL) {
        b_cap = (n < 8 ? 8 : n + 1);
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!b_cache) return;        /* allocation failed – nothing to do */
        b_cache[0] = (seq_t)0;
        b_cache[1] = (seq_t)1;
        b_cache[2] = (seq_t)1;
        b_cache[3] = (seq_t)2;
        b_cache[4] = (seq_t)2;
        b_len = 5;
    }
    if (n < b_len) return;          /* already computed */

    /* Grow capacity if needed */
    if (n >= b_cap) {
        idx_t new_cap = b_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed */
        b_cache = tmp;
        b_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = b_len; i <= n; ++i) {
        if (i < 5) continue;           /* initial values already set */
        seq_t val = b_cache[i-1] + b_cache[i-2];
        val -= b_cache[i-3];
        val += b_cache[i-5];
        b_cache[i] = val;
    }
    b_len = n + 1;
}

/* Return the term a(n) */
seq_t a(idx_t n)
{
    /* Determine row r and column c from the linear index n */
    idx_t k = n;
    idx_t r = 0;
    while (k >= r + 1) {     /* subtract successive row lengths */
        k -= (r + 1);
        r++;
    }
    idx_t c = k;             /* column within the row */

    /* Ensure required values of b are available */
    ensure_b(r);
    /* r >= c and r-c are guaranteed to be ≤ r */

    seq_t val = b_cache[r] - b_cache[c] - b_cache[r - c] + (seq_t)1;
    return val;
}
