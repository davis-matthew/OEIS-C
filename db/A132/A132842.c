/*
 * A132842  Exponentiation of A132841.
 * https://oeis.org/A132842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132842: Exponentiation of A132841
   a(0)=1.
   For n>=1: let S = Σ_{k=1}^{n-1} b[k] * a[n-k]
   Find the smallest integer b[n] > b[n-1] such that (S + b[n]) ≡ 0 (mod n)
   (For n=1 set b[1]=1).  Then
        a[n] = (S + b[n]) / n .
   This recurrence produces the least increasing logarithmic coefficients
   that make exp(A(x)) an integer sequence.  */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static size_t cap = 0;          /* capacity of caches  */
    static idx_t  maxC = 0;         /* largest computed index */

    /* ensure capacity up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap? cap<<1 : 32;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *newA = (seq_t*)realloc(A, newcap * sizeof(seq_t));
        seq_t *newB = (seq_t*)realloc(B, newcap * sizeof(seq_t));
        if (!newA || !newB) abort();
        A = newA;
        B = newB;
        cap = newcap;
    }
    /* initialize base term if needed */
    if (maxC == 0 && n >= 0) {
        A[0] = 1;
        maxC = 0;
    }
    /* compute missing terms up to n */
    for (idx_t i = maxC + 1; i <= n; ++i) {
        seq_t S = 0;
        for (idx_t k = 1; k < i; ++k)
            S += B[k] * A[i - k];
        if (i == 1) {
            B[1] = 1;
        } else {
            /* compute target remainder: S + b ≡ 0 (mod i)  =>  b ≡ -S (mod i)  */
            idx_t mod_n = (idx_t)(S % (seq_t)i);              /* 0 <= < i  */
            idx_t target = ((i - mod_n) % i);                 /* 0 <= < i  */

            /* remainder of previous b[i-1] modulo i */
            idx_t prev_rem = (idx_t)(B[i - 1] % (seq_t)i);

            /* smallest delta > 0 such that (prev_rem + delta) % i == target */
            idx_t delta = (target + i - prev_rem) % i;
            if (delta == 0) delta = i;                       /* must be strictly greater */

            B[i] = B[i - 1] + (seq_t)delta;
        }
        A[i] = (S + B[i]) / (seq_t)i;
    }
    maxC = n;
    return A[n];
}
