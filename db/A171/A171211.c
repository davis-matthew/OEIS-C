/*
 * A171211  G.f. satisfies: A(x) = (1 + x*A(2x))^8.
 * https://oeis.org/A171211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{min(8,n)} binom(8,k) * 2^{n-k} *
 *            sum_{m_1+...+m_k=n-k} ∏_{i=1}^k a(m_i) (k-fold convolution)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The following helper types are supplied by the build system. */
extern seq_t seq_zero(void);   /* not used but denotes zero type */
extern seq_t seq_one(void);    /* not used but denotes one type */

static seq_t *A_vals      = NULL;   /* a(n) values */
static seq_t *pow2_vals   = NULL;   /* 2^i values */
static seq_t *D[9]        = {NULL}; /* D[k][i] = k-fold convolution of a at i (k>=1) */

static size_t A_cap   = 0; /* current allocated capacity of A_vals etc. */
static size_t pow2_cap = 0;
static size_t D_cap   = 0;
static idx_t  computed = 0;      /* number of values already computed (max index+1) */

static const unsigned int binom8[9] = {
    1, 8, 28, 56, 70, 56, 28, 8, 1
};

/* Ensure arrays have capacity at least n+1 and fill missing 2^i values.  */
static void ensure_capacity(idx_t n) {
    if ((idx_t)computed > n) return;      /* already computed up to n         */
    idx_t cap = n + 1;
    if (cap < 16) cap = 16;                /* small initial size               */

    /* Reallocate /allocate A_vals */
    if (!A_vals) {
        A_vals = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (cap > A_cap) {
        A_vals = (seq_t *)realloc(A_vals, cap * sizeof(seq_t));
    }
    A_cap = cap;

    /* Reallocate /allocate pow2_vals */
    if (!pow2_vals) {
        pow2_vals = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (cap > pow2_cap) {
        pow2_vals = (seq_t *)realloc(pow2_vals, cap * sizeof(seq_t));
    }
    /* Compute missing powers of two */
    if (pow2_cap == 0) {
        pow2_vals[0] = (seq_t)1;
        for (idx_t i = 1; i < cap; ++i)
            pow2_vals[i] = pow2_vals[i - 1] * (seq_t)2;
    } else {
        for (idx_t i = pow2_cap; i < cap; ++i)
            pow2_vals[i] = pow2_vals[i - 1] * (seq_t)2;
    }
    pow2_cap = cap;

    /* Reallocate /allocate D[k] arrays for k = 1..8 */
    for (int k = 1; k <= 8; ++k) {
        if (!D[k]) {
            D[k] = (seq_t *)malloc(cap * sizeof(seq_t));
        } else if (cap > D_cap) {
            D[k] = (seq_t *)realloc(D[k], cap * sizeof(seq_t));
        }
    }
    D_cap = cap;

    /* Compute a(i) and all D[k][i] for i = computed ... n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t total = 0;
        for (int k = 0; k <= 8 && k <= i; ++k) {
            idx_t m = i - k;
            seq_t dk;
            if (k == 0)
                dk = (m == 0 ? (seq_t)1 : (seq_t)0);
            else
                dk = D[k][m];
            seq_t term = (seq_t)binom8[k] * dk;
            term = term * pow2_vals[m];
            total += term;
        }
        A_vals[i] = total;

        /* D[1][i] = a(i) (single convolution) */
        D[1][i] = A_vals[i];
        /* Compute higher convolutions */
        for (int k = 2; k <= 8; ++k) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= i; ++j)
                sum += A_vals[j] * D[k - 1][i - j];
            D[k][i] = sum;
        }
    }
    computed = n + 1;
}

seq_t a(idx_t n) {
    ensure_capacity(n);
    return A_vals[n];
}
