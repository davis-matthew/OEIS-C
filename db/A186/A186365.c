/*
 * A186365  Number of fixed points in all cycle-up-down permutations of
 *          {1,2,...,n}.
 * https://oeis.org/A186365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1 For n ≥ 2: a(n) = n * a(n-1) +
 *            ∑_{p=1}^{⌊(n-2)/2⌋} (-1)^p * a(n-2p-1) * C(n, 2p+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  C++ and C compatible  */
#include <stdlib.h>
#include <stdint.h>

/* Helper to compute (x * mult) / div with 128‑bit intermediate when needed */
static inline seq_t muldiv(seq_t x, unsigned long long mult, unsigned long long div)
{
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ >= 16
    if (sizeof(seq_t) <= 8) {
        __uint128_t prod = (__uint128_t)x * (__uint128_t)mult;
        return (seq_t)(prod / (__uint128_t)div);
    }
    /* fall through to normal multiplication if seq_t > 64‑bit */
#endif
    seq_t prod = x * (seq_t)mult;
    return prod / (seq_t)div;
}

/* Dynamic storage for the sequence values */
static seq_t *A = NULL;        /* array holding a(n) */
static size_t A_len = 0;       /* number of computed entries (maximum index + 1) */
static size_t A_cap = 0;       /* allocated capacity */

/* Ensure that a[n] is computed for all n up to required_n (inclusive) */
static void ensure_a(size_t required_n)
{
    if (required_n + 1 <= A_len)
        return;                /* already computed */

    /* Grow the array if necessary */
    if (!A || required_n + 1 > A_cap) {
        size_t new_cap = A_cap ? A_cap * 2 : 16;
        if (new_cap < required_n + 1)
            new_cap = required_n + 1;
        seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!newA) { /* allocation failed – abort the program */
            abort();
        }
        A = newA;
        A_cap = new_cap;
    }

    /* Initialize missing entries */
    if (!A) {
        A[0] = 0;
        if (required_n >= 1) A[1] = 1;
        A_len = (required_n >= 1) ? 2 : 1;
    }

    /* Compute terms from the current length up to required_n */
    for (size_t m = A_len; m <= required_n; ++m) {
        if (m == 0) { A[0] = 0; continue; }
        if (m == 1) { A[1] = 1; continue; }

        /* start with m * a[m-1] */
        seq_t val = (seq_t)m * A[m - 1];

        /* binomial coefficient C(m,k) iteratively */
        seq_t C = 1; /* C(m,0) */
        for (size_t k = 1; k <= m; ++k) {
            C = muldiv(C, (unsigned long long)(m - k + 1),
                       (unsigned long long)k);

            /* use only odd k ≥ 3 */
            if (k >= 3 && (k & 1)) {
                size_t shift = (k - 1) / 2;   /* p in the formula */
                seq_t term = A[m - k] * C;
                if (shift & 1)      /* sign = (-1)^p, p odd -> negative */
                    val -= term;
                else
                    val += term;
            }
        }
        A[m] = val;
    }

    A_len = required_n + 1;
}

/* The function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_a((size_t)n);
    return A[(size_t)n];
}
