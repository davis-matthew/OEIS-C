/*
 * A082423  a(1)=1, a(n)=ceiling(n/(n+1)*sum(k=1,n-1,a(k))).
 * https://oeis.org/A082423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = ceil( n/(n+1) * sum_{k=1}^{n-1} a(k) ) for n>1.
 *            Equivalent: let S(n) = sum_{k=1}^n a(k). Then a(n) = S(n-1) -
 *            floor( S(n-1) / (n+1) )
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* seq_t and idx_t are provided by the build system. */
static idx_t max_computed = 0;   /* largest n for which a(n) is known */

/* Storage for the sequence terms (a) and partial sums (S). */
#ifdef __cplusplus
static std::vector<seq_t> A;     /* A[0] unused, A[1]..A[max_computed] stored */
static std::vector<seq_t> S;     /* S[0] unused, S[1]..S[max_computed] stored */
#else
static seq_t *A = NULL;          /* a[0] unused, a[1]..a[max_computed] stored */
static seq_t *S = NULL;          /* S[0] unused, S[1]..S[max_computed] stored */
static size_t capacity = 0;      /* capacity of the arrays (for malloc) */
#endif

/* Ensure that the internal arrays can hold indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
#ifdef __cplusplus
    if ((idx_t)A.size() <= need) {
        A.resize(need + 1);         /* index 0 unused */
        S.resize(need + 1);
    }
#else
    if ((size_t)need + 1 > capacity) {
        size_t newcap = capacity ? capacity * 2 : 16;
        if (newcap < (size_t)need + 1) newcap = (size_t)need + 1;
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        S = (seq_t *)realloc(S, newcap * sizeof(seq_t));
        capacity = newcap;
    }
#endif
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    /* Ensure we have computed up to n. */
    if (max_computed < n) {
        /* Initialise storage and first term if necessary. */
        if (max_computed == 0) {
#ifdef __cplusplus
            A.resize(2);  /* indices 0 and 1; 0 unused */
            S.resize(2);
#else
            ensure_capacity(1);
#endif
            A[1] = 1;
            S[1] = 1;
            max_computed = 1;
        }

        /* Compute terms from max_computed + 1 up to n. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            ensure_capacity(i);
            seq_t prevS = S[i-1];               /* S[n-1] */
            seq_t div = prevS / (seq_t)(i + 1);   /* floor( prevS / (i+1) ) */
            seq_t ai = prevS - div;             /* a(n) = prevS - floor(prevS/(n+1)) */
            A[i] = ai;
            S[i] = prevS + ai;                  /* S[n] = S[n-1] + a(n) */
        }
        max_computed = n;
    }
    return A[n];
}
