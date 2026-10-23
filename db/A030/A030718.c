/*
 * A030718  The second list after the following procedure: starting with a
 *          list [1] and an empty list, repeatedly add the distinct values
 *          already in the first list in ascending order to the second list
 *          and add the corresponding frequencies of those values to the first
 *          list.
 * https://oeis.org/A030718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 686, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030718:  
   Two‑list process—start with A = [1] and empty B.  
   Repeatedly append the ascending distinct values of A to B, and the
   corresponding frequencies of those values to A.  a(n) is the nth term
   of B.  The implementation memoises B and A so that calls with arbitrary
   indices are handled efficiently (O(n) overall for n up to about 10⁴).*/
#include <stdlib.h>
#include <string.h>

typedef struct {
    seq_t val;
    idx_t cnt;
} pair_t;

seq_t a(idx_t n) {
    /* static memoised state */
    static seq_t *A = NULL;
    static size_t A_sz = 0, A_cap = 0;
    static seq_t *B = NULL;
    static size_t B_sz = 0, B_cap = 0;

    /* one‑time initialisation */
    if (!A) {
        A_cap = 64;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (!A) return (seq_t)0;
        A[0] = (seq_t)1;
        A_sz = 1;
    }
    if (!B) {
        B_cap = 64;
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
        if (!B) return (seq_t)0;
        B_sz = 0;
    }

    /* generate terms until we have at least n of them */
    while (B_sz < n) {
        /* Count distinct values in current A */
        size_t i;
        size_t distinct = 0;
        pair_t *pairs = (pair_t *)malloc(A_sz * sizeof(pair_t));
        if (!pairs) return (seq_t)0;
        memset(pairs, 0, A_sz * sizeof(pair_t));

        for (i = 0; i < A_sz; ++i) {
            seq_t v = A[i];
            size_t j;
            int found = 0;
            for (j = 0; j < distinct; ++j) {
                if (pairs[j].val == v) {
                    pairs[j].cnt += (idx_t)1;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                pairs[distinct].val = v;
                pairs[distinct].cnt = (idx_t)1;
                ++distinct;
            }
        }

        /* Sort pairs by val ascending (insert‑sort, small size) */
        for (i = 1; i < distinct; ++i) {
            pair_t key = pairs[i];
            size_t j = i;
            while (j > 0 && pairs[j-1].val > key.val) {
                pairs[j] = pairs[j-1];
                --j;
            }
            pairs[j] = key;
        }

        /* Append distinct values to B */
        if (B_sz + distinct > B_cap) {
            while (B_cap < B_sz + distinct) B_cap *= 2;
            B = (seq_t *)realloc(B, B_cap * sizeof(seq_t));
            if (!B) { free(pairs); return (seq_t)0; }
        }
        for (i = 0; i < distinct; ++i)
            B[B_sz++] = pairs[i].val;

        /* Append frequencies to A */
        if (A_sz + distinct > A_cap) {
            while (A_cap < A_sz + distinct) A_cap *= 2;
            A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
            if (!A) { free(pairs); return (seq_t)0; }
        }
        for (i = 0; i < distinct; ++i)
            A[A_sz++] = (seq_t)pairs[i].cnt;

        free(pairs);
    }

    return B[n-1];
}
