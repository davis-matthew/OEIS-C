/*
 * A171192  G.f. satisfies A(x) = 1/(1 - x*A(2x)^2).
 * https://oeis.org/A171192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{i=0}^{n-1} Sum_{j=0}^{n-i-1} 2^(i+j) * a(i)
 *            * a(j) * a(n-i-j-1) or equivalently a(n) = Sum_{k=0}^{n-1} a(k)
 *            * h(n-1-k) with h(m) = Sum_{i+j=m} v(i)*v(j) and v(k) = 2^k *
 *            a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *A = NULL;          /* a[0..] */
    static seq_t *V = NULL;          /* v[0..] = 2^i * a(i) */
    static seq_t *P2 = NULL;         /* pow2[0..] = 2^i */
    static seq_t *H = NULL;          /* convolution of V with itself */
    static size_t a_alloc = 0;       /* allocated size for A/V/P2 */
    static size_t h_alloc = 0;       /* allocated size for H (>= 2*a_alloc) */
    static idx_t cur_max = 0;        /* highest index already computed */
    static int init = 0;             /* initialisation flag */

    /* ------------------------------------------------------------------ */
    /* initialise storage and base values                                 */
    if (!init) {
        a_alloc = 1;                 /* enough for index 0 */
        h_alloc = 2;                 /* h[0] needed */

        A = (seq_t *)malloc(a_alloc * sizeof(seq_t));
        V = (seq_t *)malloc(a_alloc * sizeof(seq_t));
        P2 = (seq_t *)malloc(a_alloc * sizeof(seq_t));
        H = (seq_t *)malloc(h_alloc * sizeof(seq_t));

        if (!A || !V || !P2 || !H) {
            /* allocation failed; return 0 as a safety value */
            return (seq_t)0;
        }

        /* base case a(0) = 1 */
        A[0] = (seq_t)1;
        P2[0] = (seq_t)1;
        V[0] = (seq_t)1;            /* 2^0 * a(0) */
        H[0] = (seq_t)1;            /* v[0]*v[0] */

        init = 1;
    }

    /* ------------------------------------------------------------------ */
    /* ensure enough capacity for indices up to n                        */
    if ((size_t)n >= a_alloc) {
        size_t new_a = a_alloc ? a_alloc << 1 : 1;
        while (new_a <= (size_t)n) new_a <<= 1;

        A = (seq_t *)realloc(A, new_a * sizeof(seq_t));
        V = (seq_t *)realloc(V, new_a * sizeof(seq_t));
        P2 = (seq_t *)realloc(P2, new_a * sizeof(seq_t));
        if (A && V && P2) {
            /* initialise new slots to zero */
            for (size_t i = a_alloc; i < new_a; ++i) A[i] = V[i] = P2[i] = 0;
        }

        /* enlarge H if required */
        size_t new_h = new_a * 2;
        if (new_h > h_alloc) {
            H = (seq_t *)realloc(H, new_h * sizeof(seq_t));
            for (size_t i = h_alloc; i < new_h; ++i) H[i] = 0;
            h_alloc = new_h;
        }

        /* compute new powers of two */
        for (size_t i = a_alloc; i < new_a; ++i) {
            P2[i] = P2[i - 1] * (seq_t)2;
        }
        a_alloc = new_a;
    }

    /* ------------------------------------------------------------------ */
    /* compute terms up to n (if not already done)                        */
    for (idx_t k = cur_max + 1; k <= n; ++k) {
        /* compute a[k] using convolution with H */
        seq_t sum = 0;
        for (idx_t j = 0; j < k; ++j) {
            /* a[k] += a[j] * H[k-1-j] */
            sum += A[j] * H[k - 1 - j];
        }
        A[k] = sum;

        /* compute v[k] = 2^k * a[k] */
        V[k] = P2[k] * A[k];

        /* update convolution H for new index k:
           for all i < k: add 2 * V[k] * V[i] to H[k+i]
           add V[k]*V[k] to H[2*k]                           */
        for (idx_t i = 0; i < k; ++i) {
            seq_t prod = V[k] * V[i];
            prod += prod;                     /* multiply by 2 */
            H[k + i] += prod;
        }
        H[2 * k] += V[k] * V[k];
    }
    cur_max = n > cur_max ? n : cur_max;

    /* ------------------------------------------------------------------ */
    return A[n];
}
