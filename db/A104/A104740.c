/*
 * A104740  a(1) = 1; for n > 1: if n is even, a(n) = least k > 0 such that
 *          sum(i=1,n/2,a(2*i-1))/sum(j=1,n,a(j))>=1/4, or 1 if there is no
 *          such k; if n is odd, a(n) = largest k > 0 such that
 *          sum(i=1,(n+1)/2,a(2*i-1))/sum(j=1,n,a(j))<=1/3, or 1 if there is
 *          no such k.
 * https://oeis.org/A104740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, for n>1 if n even: a(n)=max(1, 4*O(n-1)-S(n-1)) if n odd
 *            : a(n)= (S(n-1)-3*O(n-1)) / 2, if this ≥1, else 1 where S(n) =
 *            sum_{j=1}^{n} a(j), O(n) = sum_{j=1, j odd}^{n} a(j)
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t capacity = 0;
static seq_t *A = NULL;   /* a values, 1-based */
static seq_t *S = NULL;   /* prefix sums, 1-based */
static seq_t *O = NULL;   /* odd-index prefix sums, 1-based */

static void grow(idx_t upto)
{
    /* Grow arrays to hold indices up to 'upto' */
    if (upto <= capacity)
        return;
    idx_t new_cap = capacity ? capacity * 2 : 16;
    if (new_cap < upto)
        new_cap = upto;
    A = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
    S = (seq_t *)realloc(S, (new_cap + 1) * sizeof(seq_t));
    O = (seq_t *)realloc(O, (new_cap + 1) * sizeof(seq_t));
    /* initialise new entries to zero */
    for (idx_t i = capacity + 1; i <= new_cap; ++i) {
        A[i] = 0;
        S[i] = 0;
        O[i] = 0;
    }
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1 */
    grow(n);
    /* If values up to n are already computed, just return */
    static idx_t computed = 0;
    if (n <= computed)
        return A[n];

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            A[1] = 1;
            S[1] = 1;
            O[1] = 1;
            continue;
        }
        if ((i & 1) == 0) { /* i even: a(n) = max(1, 4*O[n-1]-S[n-1]) */
            seq_t prod4 = O[i - 1] * (seq_t)4;
            if (prod4 > S[i - 1])
                A[i] = prod4 - S[i - 1];
            else
                A[i] = 1;
        } else { /* i odd: a(n) = floor((S[n-1]-3*O[n-1])/2) if ≥1 else 1 */
            seq_t mult3 = O[i - 1] * (seq_t)3;
            if (S[i - 1] < mult3) {
                A[i] = 1;
            } else {
                seq_t numer = S[i - 1] - mult3;
                if (numer >= 2)
                    A[i] = numer / (seq_t)2;
                else
                    A[i] = 1;
            }
        }
        S[i] = S[i - 1] + A[i];
        if ((i & 1) == 1)
            O[i] = O[i - 1] + A[i];
        else
            O[i] = O[i - 1];
    }
    computed = n;
    return A[n];
}
