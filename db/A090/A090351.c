/*
 * A090351  G.f. satisfies A^3 = BINOMIAL(A^2).
 * https://oeis.org/A090351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the sequence A090351 defined by the functional equation *
 *            A(x)^3 = (A(x/(1-x)))^2 / (1-x). * This implementation computes
 *            the terms via the recurrence * a_n = P_n + Q_n + T_{n-1} - S_n *
 *            where * P_n = sum_{i=1}^{n-1} a_i a_{n-i} (stored in conv2[n]
 *            before update) * Q_n = sum_{k=1}^{n-1} co...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ensure the dynamic array has at least 'needed' elements */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (new_cap > *cap)
            memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *a_seq      = NULL; /* value sequence a(0), a(1), ... */
    static seq_t *conv2      = NULL; /* convolution of a with itself: conv2[k] = Σ a_i a_{k-i} */
    static size_t a_cap      = 0;
    static size_t conv2_cap  = 0;
    static seq_t cum_d       = 0;     /* cumulative sum of d_m : Σ_{m=0}^{m} d_m */
    static size_t next_needed = 1;    /* next index that needs to be computed (next_needed > a_cap-1) */

    /* initialisation */
    if (a_seq == NULL) {
        a_cap = 4;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_seq) abort();
        memset(a_seq, 0, a_cap * sizeof(seq_t));

        conv2_cap = 4;
        conv2 = (seq_t *)malloc(conv2_cap * sizeof(seq_t));
        if (!conv2) abort();
        memset(conv2, 0, conv2_cap * sizeof(seq_t));

        a_seq[0] = (seq_t)1;
        conv2[0] = (seq_t)1;     /* a0 * a0 */
        cum_d = (seq_t)1;        /* d0 = a0^2 = 1 */
        next_needed = 1;
    }

    /* compute up to desired n */
    while (next_needed <= (size_t)n) {
        idx_t k = (idx_t)next_needed;

        /* ensure capacity for this index */
        ensure_capacity(&a_seq,   &a_cap,   k + 1);
        ensure_capacity(&conv2,  &conv2_cap, 2 * k + 1);   /* we need indices up to 2*k */

        /* old conv2 value before adding a_k */
        seq_t old_conv2_k = conv2[k];

        /* -------------------- compute Q_k -------------------- */
        seq_t Q = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(k-1, 0) */
        for (idx_t i = 1; i < k; ++i) {  /* k-1 iterations max */
            Q += conv2[i] * binom;
            /* update binom for next i */
            binom = binom * (seq_t)(k - i) / (seq_t)i;
        }

        /* -------------------- compute S_k -------------------- */
        seq_t S = (seq_t)0;
        for (idx_t p = 0; p < k; ++p) {
            S += a_seq[p] * conv2[k - p];
        }

        /* -------------------- compute a_k -------------------- */
        seq_t a_k = old_conv2_k + Q + cum_d - S;
        a_seq[k] = a_k;

        /* -------------------- update conv2 with new a_k -------------------- */
        /* conv2[k] accounts for pairs (k,0) and (0,k) */
        conv2[k] = old_conv2_k + ((seq_t)2) * a_k;

        /* pairs where one index is k and the other is 1..k-1 */
        for (idx_t i = 1; i < k; ++i) {
            seq_t prod = a_k * a_seq[i];
            conv2[k + i] += prod + prod;   /* add twice for ordered pairs (i,k) and (k,i) */
        }

        /* pair (k,k) */
        conv2[2 * k] += a_k * a_k;

        /* -------------------- compute d_k and update cumulative sum -------------------- */
        seq_t d_k = conv2[k] + Q;          /* conv2[k] already updated with 2*a_k */
        cum_d += d_k;

        /* advance to next index */
        ++next_needed;
    }

    return a_seq[(size_t)n];
}
