/*
 * A195065  Numbers n such that BCR(n) is greater than n, where BCR =
 *          binary-complement-and-reverse = A036044.
 * https://oeis.org/A195065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m such that BCR(m) > m, where BCR(m) is
 *            the binary-complement-and-reverse of m. a(1) = 0
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#ifdef __cplusplus
#include <new>       /* std::nothrow */
#endif

/* Compute BCR of a non‑negative integer m.
   The input is given as an unsigned integer type idx_t; it is
   converted to seq_t for manipulation.  For m == 0,
   BCR(0) = 1. */
static inline seq_t bcr(idx_t m)
{
    seq_t v = (seq_t)m;
    if (v == (seq_t)0) return (seq_t)1;

    /* Determine bit length of v. */
    int len = 0;
    seq_t tmp = v;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Build the complemented‑reversed value. */
    seq_t res = (seq_t)0;
    for (int i = 0; i < len; ++i) {
        seq_t bit = (v >> i) & (seq_t)1;
        res = (res << 1) | (bit ^ (seq_t)1);
    }
    return res;
}

/* ----------- dynamic storage for the sequence ----------- */
static seq_t *terms      = NULL;   /* cached values */
static size_t terms_len  = 0;      /* number of terms cached */
static size_t terms_cap  = 0;      /* allocated capacity */
static idx_t  max_checked = 1;     /* next m to examine */

/* Grow the terms buffer when needed. */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new (std::nothrow) seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr) return;               /* allocation failure – caller will see incomplete data */
    for (size_t i = 0; i < terms_len; ++i) new_arr[i] = terms[i];
#ifdef __cplusplus
    delete[] terms;
#else
    free(terms);
#endif
    terms   = new_arr;
    terms_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the first term (0) is cached. */
    if (terms_len == 0) {
        if (terms_cap == 0) grow_terms();
        terms[terms_len++] = (seq_t)0;
    }

    /* Compute additional terms until we have at least n of them. */
    while (terms_len < n) {
        for (;;) {
            idx_t m = max_checked;                 /* current integer to test */
            seq_t b = bcr(m);                      /* BCR(m) */
            if (b > (seq_t)m) {                    /* qualifies */
                if (terms_len == terms_cap) grow_terms();
                terms[terms_len++] = (seq_t)m;
                max_checked = (idx_t)(m + 1);
                break;
            }
            ++max_checked;
        }
    }

    return terms[n - 1];      /* a(1) is the first term stored */
}
