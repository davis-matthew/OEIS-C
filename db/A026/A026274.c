/*
 * A026274  Greatest k such that s(k) = n, where s = A026272.
 * https://oeis.org/A026274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026274: greatest k such that s(k) = n, where s = A026272.
   Implemented by generating the upper Wythoff sequence.
   a(n) = floor((n+1)*φ²) – 2  with φ = (1+√5)/2. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state to make calls in any order O(1) amortized */
    static seq_t *upper = NULL;          /* array of b_k = upper Wythoff terms   */
    static size_t upper_cap = 0;         /* current capacity of upper array      */
    static size_t upper_len = 0;         /* number of b_k computed so far        */

    static unsigned char *used = NULL;   /* bitmask of integers that already appear */
    static size_t used_cap = 0;          /* current capacity of used array        */
    static idx_t used_idx = 1;           /* next candidate for a_k                */

    /* We need b_{n+1} for the result */
    idx_t need_k = n + 1;

    /* Ensure the upper array is large enough */
    if (need_k > upper_cap) {
        size_t new_cap = upper_cap ? upper_cap : 64;
        while (new_cap <= need_k) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(upper, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        upper = tmp;
        upper_cap = new_cap;
    }

    /* Generate Wythoff pairs until we have b_{need_k} */
    while (upper_len < need_k) {
        idx_t k = upper_len + 1;          /* current Wythoff index   */

        /* Find the smallest unused integer → a_k */
        for (;;) {
            if (used_idx >= used_cap) {   /* grow used array if necessary */
                size_t new_cap = used_cap ? used_cap : 128;
                while (new_cap <= used_idx) new_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
                if (!tmp) abort();
                /* initialise new part to zero */
                memset(tmp + used_cap, 0, new_cap - used_cap);
                used = tmp;
                used_cap = new_cap;
            }
            if (!used[used_idx]) break;   /* found a_k */
            ++used_idx;
        }
        idx_t a_k = used_idx;            /* a_k found */
        used[a_k] = 1;                   /* mark a_k as used   */

        /* Compute b_k = a_k + k and mark it */
        idx_t b_k = a_k + k;
        while (b_k >= used_cap) {        /* grow used array if needed for b_k */
            size_t new_cap = used_cap ? used_cap : 128;
            while (new_cap <= b_k) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
            if (!tmp) abort();
            memset(tmp + used_cap, 0, new_cap - used_cap);
            used = tmp;
            used_cap = new_cap;
        }
        used[b_k] = 1;

        /* Store the upper sequence value */
        upper[upper_len] = (seq_t)b_k;
        ++upper_len;
    }

    /* a(n) = b_{n+1} – 2 */
    seq_t result = upper[need_k - 1] - (seq_t)2;
    return result;
}
