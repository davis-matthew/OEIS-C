/*
 * A239107  Number of hybrid 4-ary trees with n internal nodes.
 * https://oeis.org/A239107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A239107: Number of hybrid 4‑ary trees with n internal nodes
   Generating function:  A(x) = (1 + x*A(x)^3) * (1 + x*A(x)^4)
   Recurrence implementation:
     a(0) = 1
     for n ≥ 1:
        a(n) = [x^{n-1}]A^3 + [x^{n-1}]A^4 + [x^{n-2}]A^7
   The code maintains the convolution sequences Ck[m] = coeff of x^m in A^k
   for k = 2..7 and updates them incrementally.  */
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ version – use std::vector so that arbitrary‑precision classes are
       default‑constructed correctly. */
    using Vec = std::vector<seq_t>;
    static Vec A, C2, C3, C4, C5, C6, C7;
    static idx_t computed = (idx_t)-1;

    if (computed == (idx_t)-1) {
        A.push_back(seq_t(1));
        C2.push_back(seq_t(1));
        C3.push_back(seq_t(1));
        C4.push_back(seq_t(1));
        C5.push_back(seq_t(1));
        C6.push_back(seq_t(1));
        C7.push_back(seq_t(1));
        computed = 0;
    }

    for (size_t k = computed + 1; k <= (size_t)n; ++k) {
        seq_t sum3 = seq_t(0);
        for (size_t i = 0; i <= k - 1; ++i)
            sum3 += A[i] * C2[(k - 1) - i];

        seq_t sum4 = seq_t(0);
        for (size_t i = 0; i <= k - 1; ++i)
            sum4 += A[i] * C3[(k - 1) - i];

        seq_t sum7 = seq_t(0);
        if (k >= 2)
            for (size_t i = 0; i <= k - 2; ++i)
                sum7 += A[i] * C6[(k - 2) - i];

        A.push_back(sum3 + sum4 + sum7);

        /* update convolution tables */
        seq_t c2k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c2k += A[i] * A[k - i];
        C2.push_back(c2k);

        seq_t c3k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c3k += A[i] * C2[k - i];
        C3.push_back(c3k);

        seq_t c4k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c4k += A[i] * C3[k - i];
        C4.push_back(c4k);

        seq_t c5k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c5k += A[i] * C4[k - i];
        C5.push_back(c5k);

        seq_t c6k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c6k += A[i] * C5[k - i];
        C6.push_back(c6k);

        seq_t c7k = seq_t(0);
        for (size_t i = 0; i <= k; ++i)
            c7k += A[i] * C6[k - i];
        C7.push_back(c7k);
    }

    computed = n;
    return A[(size_t)n];
#else
    /* C version – seq_t is an integer type, so plain malloc works. */
    static seq_t *A  = NULL;
    static seq_t *C2 = NULL;
    static seq_t *C3 = NULL;
    static seq_t *C4 = NULL;
    static seq_t *C5 = NULL;
    static seq_t *C6 = NULL;
    static seq_t *C7 = NULL;
    static idx_t  computed = (idx_t)-1;
    static size_t cap = 0;

    if (computed == (idx_t)-1) {
        /* first call – allocate space for one element */
        cap = 1;
        A  = (seq_t *)malloc(cap * sizeof(seq_t));
        C2 = (seq_t *)malloc(cap * sizeof(seq_t));
        C3 = (seq_t *)malloc(cap * sizeof(seq_t));
        C4 = (seq_t *)malloc(cap * sizeof(seq_t));
        C5 = (seq_t *)malloc(cap * sizeof(seq_t));
        C6 = (seq_t *)malloc(cap * sizeof(seq_t));
        C7 = (seq_t *)malloc(cap * sizeof(seq_t));

        A[0]  = (seq_t)1;
        C2[0] = (seq_t)1;
        C3[0] = (seq_t)1;
        C4[0] = (seq_t)1;
        C5[0] = (seq_t)1;
        C6[0] = (seq_t)1;
        C7[0] = (seq_t)1;
        computed = 0;
    } else if (cap <= (size_t)n + 1) {
        /* grow all arrays to hold up to n */
        size_t new_cap = (size_t)n + 1;
        A  = (seq_t *)realloc(A,  new_cap * sizeof(seq_t));
        C2 = (seq_t *)realloc(C2, new_cap * sizeof(seq_t));
        C3 = (seq_t *)realloc(C3, new_cap * sizeof(seq_t));
        C4 = (seq_t *)realloc(C4, new_cap * sizeof(seq_t));
        C5 = (seq_t *)realloc(C5, new_cap * sizeof(seq_t));
        C6 = (seq_t *)realloc(C6, new_cap * sizeof(seq_t));
        C7 = (seq_t *)realloc(C7, new_cap * sizeof(seq_t));
        /* initialise new entries to zero */
        for (size_t i = cap; i < new_cap; ++i) {
            A[i] = C2[i] = C3[i] = C4[i] = C5[i] = C6[i] = C7[i] = (seq_t)0;
        }
        cap = new_cap;
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t sum3 = (seq_t)0;
        for (idx_t i = 0; i <= k - 1; ++i)
            sum3 += A[i] * C2[(k - 1) - i];

        seq_t sum4 = (seq_t)0;
        for (idx_t i = 0; i <= k - 1; ++i)
            sum4 += A[i] * C3[(k - 1) - i];

        seq_t sum7 = (seq_t)0;
        if (k >= 2)
            for (idx_t i = 0; i <= k - 2; ++i)
                sum7 += A[i] * C6[(k - 2) - i];

        A[k] = sum3 + sum4 + sum7;

        /* update convolution tables */
        seq_t c2k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c2k += A[i] * A[k - i];
        C2[k] = c2k;

        seq_t c3k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c3k += A[i] * C2[k - i];
        C3[k] = c3k;

        seq_t c4k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c4k += A[i] * C3[k - i];
        C4[k] = c4k;

        seq_t c5k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c5k += A[i] * C4[k - i];
        C5[k] = c5k;

        seq_t c6k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c6k += A[i] * C5[k - i];
        C6[k] = c6k;

        seq_t c7k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c7k += A[i] * C6[k - i];
        C7[k] = c7k;
    }

    computed = n;
    return A[(size_t)n];
#endif
}
