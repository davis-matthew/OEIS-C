/*
 * A217473  Product of the first n+1 odd-indexed Lucas numbers A000032.
 * https://oeis.org/A217473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=0..n} L(2*i+1) (product of odd‑indexed Lucas
 *            numbers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS A217473.
   Uses memoisation; works with any seq_t and idx_t supplied by the build. */
seq_t a(idx_t n)
{
    /* Memoised arrays for Lucas numbers and products */
    static seq_t *lucas = NULL;   /* stores Lucas L(k) for k = 0..max_needed */
    static size_t lucas_sz = 0;
    static seq_t *prod  = NULL;   /* stores a(i) for i = 0..max_computed */
    static size_t prod_len = 0;

    /* ------------------------------------------------------------- */
    /* 1. Ensure we have Lucas numbers up to index 2*n+1            */
    /* ------------------------------------------------------------- */
    size_t lucas_needed = (size_t)(2 * n + 1);   /* maximum k needed */

    if (lucas_sz == 0) {                 /* first call: initialise */
        lucas = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!lucas) return (seq_t)0;
        lucas[0] = (seq_t)2;              /* L(0) */
        lucas[1] = (seq_t)1;              /* L(1) */
        lucas_sz = 2;
    }

    if (lucas_sz <= lucas_needed) {
        size_t new_sz = lucas_needed + 1;
        seq_t *tmp = (seq_t*)realloc(lucas, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        lucas = tmp;
        for (size_t i = lucas_sz; i < new_sz; ++i)
            lucas[i] = lucas[i-1] + lucas[i-2];
        lucas_sz = new_sz;
    }

    /* ------------------------------------------------------------- */
    /* 2. Ensure we have product values up to a(n)                   */
    /* ------------------------------------------------------------- */
    if (prod_len == 0) {                 /* first call: initialise */
        prod = (seq_t*)malloc(sizeof(seq_t));
        if (!prod) return (seq_t)0;
        prod[0] = (seq_t)1;              /* a(0) = 1 */
        prod_len = 1;
    }

    if (prod_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(prod, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        prod = tmp;
        for (size_t i = prod_len; i <= (size_t)n; ++i) {
            /* prod[i] = prod[i-1] * L(2*i+1) */
            seq_t odd_lucas = lucas[2 * i + 1];
            prod[i] = prod[i-1] * odd_lucas;
        }
        prod_len = new_len;
    }

    return prod[(size_t)n];
}
