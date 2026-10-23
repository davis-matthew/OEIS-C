/*
 * A023597  Convolution of A023532 and Lucas numbers.
 * https://oeis.org/A023597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} b(k)·L_{n+1−k}, where b(k)=0 if k = t(t+3)/2
 *            for some t≥1 =1 otherwise, and L_m are the Lucas numbers with
 *            L_1=1, L_2=3, L_m=L_{m-1}+L_{m-2}
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Static tables that are expanded on the first call and subsequent calls */
    static seq_t *L      = NULL;     /* Lucas numbers, L[1…]            */
    static seq_t *cumL   = NULL;     /* Prefix sums of Lucas: cumL[i] = Σ_{j=1}^{i} L_j */
    static idx_t  L_cap  = 0;        /* current capacity of L and cumL      */
    static idx_t *zero   = NULL;     /* indices k where b(k)=0, sorted      */
    static size_t zero_cap = 0;      /* capacity of zero table              */
    static size_t zero_sz  = 0;      /* current number of zeros             */
    static idx_t  next_k   = 1;      /* next t to generate a zero index     */

    /* ---- ensure capacity for Lucas numbers and prefix sums ---- */
    idx_t needed = n + 2;          /* we may need up to index n at least */
    if (needed > L_cap) {
        idx_t new_cap = L_cap ? L_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;

        L    = (seq_t*)realloc(L,    new_cap * sizeof(seq_t));
        cumL = (seq_t*)realloc(cumL, new_cap * sizeof(seq_t));
        L_cap = new_cap;

        /* Re‑compute all Lucas numbers and prefix sums up to L_cap-1 */
        cumL[0] = 0;
        if (L_cap > 1) {
            L[1] = 1;
            cumL[1] = 1;
        }
        if (L_cap > 2) {
            L[2] = 3;
            cumL[2] = 1 + 3;
        }
        for (idx_t i = 3; i < L_cap; ++i) {
            L[i]    = L[i-1] + L[i-2];
            cumL[i] = cumL[i-1] + L[i];
        }
    }

    /* ---- grow the zero index list up to n ---- */
    while (1) {
        idx_t cand = next_k * (next_k + 3) / 2;   /* candidate zero index */
        if (cand > n) break;

        if (zero_sz == zero_cap) {
            idx_t new_zero_cap = zero_cap ? zero_cap << 1 : 8;
            zero = (idx_t*)realloc(zero, new_zero_cap * sizeof(idx_t));
            zero_cap = new_zero_cap;
        }
        zero[zero_sz++] = cand;
        ++next_k;
    }

    /* ---- compute a(n) as sum of Lucas numbers minus zero contributions ---- */
    seq_t result = cumL[n];
    for (size_t i = 0; i < zero_sz; ++i) {
        idx_t z = zero[i];
        if (z > n) break;                /* remaining zeros are too large */
        result -= L[n + 1 - z];
    }

    return result;
}
