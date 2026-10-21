/*
 * A063011  Ordered products of the sides of primitive Pythagorean triangles.
 * https://oeis.org/A063011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the sides of the n‑th smallest primitive
 *            Pythagorean triple (ordered by that product). Here the function
 *            generates all primitive triples up to a sufficient bound, stores
 *            all products, and returns the n‑th smallest product
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, realloc, free, qsort */
#include "../utils/gcd.h"           /* gcd(seq_t, seq_t) */

#if defined(__cplusplus)
    #include <algorithm>           /* std::nth_element */
#endif

/* comparison function for qsort (used only when compiling as C) */
#ifndef __cplusplus
static int compare_seq(const void *p1, const void *p2)
{
    const seq_t *v1 = (const seq_t *)p1;
    const seq_t *v2 = (const seq_t *)p2;
    return (*v1 < *v2) ? -1 : (*v1 > *v2) ? 1 : 0;
}
#endif

/* --------------------------------------------------------------------- */
/*  Internal data and helper                                           */
/* --------------------------------------------------------------------- */
static seq_t *products     = NULL;  /* array holding all products generated   */
static size_t prod_len     = 0;     /* actual number of products in array   */
static size_t prod_cap     = 0;     /* allocated capacity of the array      */

/* Re‑generate all primitive triples and their products until we have at
   least 'needed' products.  The bound 'm_limit' is doubled as needed.
   The function releases any previously allocated array; the new array
   remains after the function returns.  */
static void recompute_to(size_t needed)
{
    idx_t m_limit = 200;            /* initial limit on the Euclid parameter m      */
    while (1) {
        size_t cap = 1024;          /* initial capacity of the product array       */
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tmp) return;                  /* allocation failure – give up                */
        size_t len = 0;                           /* number of products collected              */

        for (idx_t m = 2U; m <= m_limit; ++m) {
            seq_t m_sq = (seq_t)m * (seq_t)m;
            for (idx_t n = 1U; n < m; ++n) {
                if (((m - n) & 1U) == 0U) continue;                     /* parity check */
                if (gcd((seq_t)m, (seq_t)n) != 1U) continue;            /* coprime check */

                seq_t a_val = m_sq - (seq_t)n * (seq_t)n;          /* a = m^2 - n^2        */
                seq_t b_val = (seq_t)2 * (seq_t)m * (seq_t)n;      /* b = 2mn              */
                seq_t c_val = m_sq + (seq_t)n * (seq_t)n;          /* c = m^2 + n^2        */
                seq_t prod   = a_val * b_val * c_val;              /* product a*b*c        */

                if (len == cap) {                                   /* grow array          */
                    cap <<= 1;
                    seq_t *tmp2 = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                    if (!tmp2) { free(tmp); return; }
                    tmp = tmp2;
                }
                tmp[len++] = prod;
            }
        }

        if (len >= needed) {                                      /* enough products found */
            /* Free any previously stored array */
            free(products);
            products = tmp;
            prod_len = len;
            prod_cap = cap;
            return;
        }

        /* Not enough – try a larger bound on m */
        free(tmp);
        m_limit <<= 1;
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;             /* undefined – return zero for safety            */

    /* Ensure we have at least n products stored */
    if (products == NULL || n > prod_len) {
        recompute_to((size_t)n);
        /* If recompute failed, products could be NULL – in that case return 0 */
        if (products == NULL) return 0;
    }

    /* Bring the n‑th smallest product to position n‑1 */
#if defined(__cplusplus)
    std::nth_element(products, products + (size_t)n - 1, products + prod_len);
#else
    /* For C builds we resort the whole array – simpler and safe for POD seq_t */
    qsort(products, prod_len, sizeof(seq_t), compare_seq);
#endif

    return products[(size_t)n - 1];
}
