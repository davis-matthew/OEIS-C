/*
 * A215452  a(1)=1, a(n) = (sum of previous terms) mod (a(n-1) + n).
 * https://oeis.org/A215452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1003, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215452
   Formula: a(1) = 1,
            a(n) = (a(1)+...+a(n-1)) mod (a(n-1)+n)   for n ≥ 2
   Offset: 1
   The function memoises values so calls can be made in any order.
*/
#include <stdlib.h>

static seq_t *S  = NULL;          /* array of a(i)   (1‑based) */
static seq_t *P  = NULL;          /* prefix sums: P[i] = sum_{k=1}^{i} a(k) */
static idx_t  last = 0;           /* largest n that has been computed */
static size_t cap   = 0;           /* current capacity of the arrays */

/* Ensure the arrays hold at least n elements (index 1..n). */
static void ensure_capacity(idx_t n_needed)
{
    if ((size_t)n_needed <= cap)
        return;                       /* already large enough */

    size_t new_cap = cap ? cap * 2 : 8;
    while (new_cap < (size_t)n_needed)
        new_cap *= 2;

    S = (seq_t *)realloc(S, (new_cap + 1) * sizeof(seq_t));
    P = (seq_t *)realloc(P, (new_cap + 1) * sizeof(seq_t));
    cap = new_cap;
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n<1, but return 0 */

    ensure_capacity(n);                 /* make sure arrays are big enough */

    if (n <= last)                      /* already computed */
        return S[n];

    /* Compute terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i == 1)                     /* initial value */
        {
            S[1] = (seq_t)1;
            P[1] = S[1];
        }
        else
        {
            seq_t denom = S[i - 1] + (seq_t)i;
            seq_t val   = P[i - 1] % denom;
            S[i]        = val;
            P[i]        = P[i - 1] + val;
        }
    }

    last = n;
    return S[n];
}
