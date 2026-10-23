/*
 * A134055  a(n) = Sum_{k=1..n} C(n-1,k-1) * S2(n,k) for n>0, a(0)=1, where
 *          S2(n,k) = A048993(n,k) are Stirling numbers of the 2nd kind.
 * https://oeis.org/A134055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} C(n-1,k-1) * S2(n,k) (Stirling 2nd kind)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;            /* cache of a(n) */
static size_t A_cap = 0;           /* capacity of A   */

static seq_t *S = NULL;            /* current Stirling row  */
static size_t S_cap = 0;           /* capacity of S     */
static idx_t   S_last_n = 0;       /* last n that was processed   */

seq_t a(idx_t n)
{
    /* ----- initialise cache for a(0) ----- */
    if (!A) {
#if defined(__cplusplus)
        A = new seq_t[1];
#else
        A = (seq_t*)malloc(sizeof(seq_t));
#endif
        if (!A) abort();
        A[0] = (seq_t)1;          /* a(0) = 1 */
        A_cap = 1;
    }
    if (n == 0) return A[0];

    /* ----- grow cache for a(n) if necessary ----- */
    if ((idx_t)n >= (idx_t)A_cap) {
        size_t new_cap = (size_t)n + 1;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
#else
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t)*new_cap);
#endif
        if (!tmp) abort();
        for (size_t i = 0; i < A_cap; ++i) tmp[i] = A[i];
#if defined(__cplusplus)
        delete[] A;
#else
        free(A);
#endif
        A = tmp;
        A_cap = new_cap;
    }

    /* ----- initialise Stirling table on first call ----- */
    if (!S) {
#if defined(__cplusplus)
        S = new seq_t[1];
#else
        S = (seq_t*)malloc(sizeof(seq_t));
#endif
        if (!S) abort();
        S[0] = (seq_t)1;          /* S2(0,0) = 1 */
        S_cap = 1;
    }

    /* ----- compute rows up to n, reusing cached values ----- */
    for (idx_t i = S_last_n + 1; i <= n; ++i) {
        if ((idx_t)i >= (idx_t)S_cap) {
            size_t new_cap = (size_t)i + 1;
#if defined(__cplusplus)
            seq_t *tmp = new seq_t[new_cap];
#else
            seq_t *tmp = (seq_t*)malloc(sizeof(seq_t)*new_cap);
#endif
            if (!tmp) abort();
            for (size_t k = 0; k < S_cap; ++k) tmp[k] = S[k];
#if defined(__cplusplus)
            delete[] S;
#else
            free(S);
#endif
            S = tmp;
            S_cap = new_cap;
        }

        /* compute S2(i,k) in place: S[k] = k*S[k] + S[k-1]   */
        S[i] = (seq_t)0;            /* new entry starts at 0 */
        for (idx_t k = i; k >= 1; --k) {
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = (seq_t)0;            /* S2(i,0) = 0 for i>0 */

        /* compute a(i) = Σ_{k=1..i} C(i-1,k-1) * S2(i,k) */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;     /* C(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            sum += binom * S[k];
            if (k < i) {
                binom = binom * (seq_t)(i - k) / (seq_t)k;
            }
        }
        A[i] = sum;
        S_last_n = i;
    }

    return A[n];
}
