/*
 * A110837  Number of ways to fold a strip of n stamps taking account of order
 *          and direction of folds.
 * https://oeis.org/A110837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{0<k<n} max{a(k), a(n-k)}, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;            /* array of a(n)  */
static seq_t *P = NULL;            /* prefix sums of a(n) */
static idx_t  sz   = 0;            /* highest index computed */
static idx_t  cap  = 0;            /* current capacity of arrays */

static void ensure_capacity(idx_t n)
{
    if (n < cap) return;
    idx_t new_cap = cap ? cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;
    A  = (seq_t *)realloc(A, sizeof(seq_t) * new_cap);
    P  = (seq_t *)realloc(P, sizeof(seq_t) * new_cap);
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    if (sz == 0) {                 /* first call: initialise */
        sz = 1;
        ensure_capacity(1);
        A[1] = 1;
        P[0] = 0;
        P[1] = 1;
    }

    if (n > sz) {
        ensure_capacity(n);
        for (idx_t i = sz + 1; i <= n; ++i) {
            idx_t L = (i + 1) / 2;                        /* ceil(i/2) */
            seq_t sum1 = P[i - 1] - P[i - L];             /* prefix[i-1] - prefix[i-L] */
            seq_t sum2 = P[i - 1] - P[L - 1];             /* prefix[i-1] - prefix[L-1] */
            seq_t S    = sum1 + sum2;
            A[i] = S << 1;                               /* multiply by 2 */
            P[i] = P[i - 1] + A[i];
        }
        sz = n;
    }

    return A[n];
}
