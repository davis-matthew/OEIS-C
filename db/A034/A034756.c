/*
 * A034756  Dirichlet convolution of 3^(n-1) with Bell numbers.
 * https://oeis.org/A034756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d | n} 3^(d-1) * B_{(n/d)-1} where B_k is the k-th
 *            Bell number (B_0=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses Dirichlet convolution of 3^(n-1) with Bell numbers.
   It caches Bell numbers, powers of 3, and previously computed terms
   so that each call is O(n/√n) for a(n) plus O(n^2) for Bell number
   generation up to n.  The code is valid both in C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Cached Bell numbers B_k (k ≥ 0) */
#if defined(__cplusplus)
static std::vector<seq_t> bell;   /* bell[k] = Bell number B_k */
static size_t last_bell = 0;      /* highest index already computed */
#else
static seq_t *bell = NULL;
static size_t bell_cap = 0;
static size_t last_bell = 0;
#endif

/* Cached powers of 3: pow3[d] = 3^(d-1) for d ≥ 1 */
#if defined(__cplusplus)
static std::vector<seq_t> pow3;
static size_t last_pow3 = 0;
#else
static seq_t *pow3 = NULL;
static size_t pow3_cap = 0;
static size_t last_pow3 = 0;
#endif

/* Cached convolution results a(n) */
#if defined(__cplusplus)
static std::vector<seq_t> a_arr;  /* a_arr[n] = a(n) */
static size_t last_a = 0;
#else
static seq_t *a_arr = NULL;
static size_t a_cap = 0;
static size_t last_a = 0;
#endif

/* Utility: ensure Bell array covers index n */
static void ensure_bell_size(size_t n) {
#if defined(__cplusplus)
    if (bell.size() <= n) bell.resize(n + 1);
#else
    if (bell_cap <= n) {
        size_t new_cap = n + 1;
        if (bell == NULL)
            bell = (seq_t *)calloc(new_cap, sizeof(seq_t));
        else
            bell = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (bell == NULL) abort();
        bell_cap = new_cap;
    }
#endif
}
/* Utility: ensure pow3 array covers index n */
static void ensure_pow3_size(size_t n) {
#if defined(__cplusplus)
    if (pow3.size() <= n) pow3.resize(n + 1);
#else
    if (pow3_cap <= n) {
        size_t new_cap = n + 1;
        if (pow3 == NULL)
            pow3 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        else
            pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        if (pow3 == NULL) abort();
        pow3_cap = new_cap;
    }
#endif
}
/* Utility: ensure a_arr array covers index n */
static void ensure_a_size(size_t n) {
#if defined(__cplusplus)
    if (a_arr.size() <= n) a_arr.resize(n + 1);
#else
    if (a_cap <= n) {
        size_t new_cap = n + 1;
        if (a_arr == NULL)
            a_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        else
            a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (a_arr == NULL) abort();
        a_cap = new_cap;
    }
#endif
}

/* Compute Bell numbers B_0 .. B_N */
static void compute_bell_up_to(size_t N) {
    if (N == 0) {
        /* Ensure B_0 is defined */
        ensure_bell_size(0);
#if defined(__cplusplus)
        if (bell[0] == 0) bell[0] = (seq_t)1;
#else
        if (bell[0] == 0) bell[0] = (seq_t)1;
#endif
        last_bell = 0;
        return;
    }
    ensure_bell_size(N);
#if defined(__cplusplus)
    if (bell[0] == 0) bell[0] = (seq_t)1;
#else
    if (bell[0] == 0) bell[0] = (seq_t)1;
#endif
    size_t start = last_bell + 1;
    if (start < 1) start = 1;        /* Bell[0] already set */

    for (size_t i = start; i <= N; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;           /* binom(i-1,0) */
        for (size_t k = 0; k < i; ++k) {
            sum += binom * bell[k];
            if (k + 1 < i)
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
        }
        bell[i] = sum;
    }
    last_bell = N;
}

/* Compute powers of 3 up to exponent n (i.e., pow3[n] = 3^(n-1)) */
static void compute_pow3_up_to(size_t n) {
    if (n == 0) {
        ensure_pow3_size(1);          /* at least index 1 needed */
#if defined(__cplusplus)
        if (pow3[1] == 0) pow3[1] = (seq_t)1;
#else
        if (pow3[1] == 0) pow3[1] = (seq_t)1;
#endif
        last_pow3 = 1;
        return;
    }
    ensure_pow3_size(n);
#if defined(__cplusplus)
    if (pow3[1] == 0) pow3[1] = (seq_t)1;
#else
    if (pow3[1] == 0) pow3[1] = (seq_t)1;
#endif
    size_t start = last_pow3 + 1;
    if (start < 2) start = 2;         /* pow3[1] already set */
    for (size_t i = start; i <= n; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    last_pow3 = n;
}

/* Compute the convolution values a(1)..a(n) using cached data */
static void compute_a_up_to(size_t N) {
    if (N == 0) return;             /* a(0) not used */
    compute_bell_up_to(N - 1);       /* we need B_{(n/d)-1} up to n-1 */
    compute_pow3_up_to(N);           /* powers up to n */
    ensure_a_size(N);

    size_t start = last_a + 1;
    if (start < 1) start = 1;        /* a(1) first */

    for (size_t m = start; m <= N; ++m) {
        seq_t res = 0;
        size_t root = (size_t)sqrt((double)m);
        for (size_t d = 1; d <= root; ++d) {
            if (m % d == 0) {
                size_t d2 = m / d;
                res += pow3[d] * bell[d2 - 1];
                if (d2 != d) {
                    res += pow3[d2] * bell[d - 1];
                }
            }
        }
        a_arr[m] = res;
    }
    last_a = N;
}

/* Main function requested by the problem */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* offset is 1, but guard anyway */
    compute_a_up_to((size_t)n);
    return a_arr[(size_t)n];
}
