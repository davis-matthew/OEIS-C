/*
 * A074587  Sum of the coefficients of the n-th Moebius polynomial, M(n,x),
 *          where M(n,-1) = mu(n), the Moebius function of n.
 * https://oeis.org/A074587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 1 + sum_{i=1}^{n-1} floor(n / i) * a(i) (a(1)=1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays for memoization */
    static seq_t *A = NULL;     /* a[0] unused, a[0] = 0 as dummy */
    static seq_t *pref = NULL;  /* prefix sums: pref[i] = a(1)+…+a(i) */
    static size_t sz   = 0;     /* number of elements stored (including dummy) */
    static size_t cap  = 0;     /* allocated capacity */

    /* initialise on first call */
    if (sz == 0) {
        cap = 1;
        A    = (seq_t *)malloc(cap * sizeof(seq_t));
        pref = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A || !pref) { /* allocation failed – return 0 (unlikely) */
            return (seq_t)0;
        }
        A[0]    = (seq_t)0;
        pref[0] = (seq_t)0;
        sz      = 1;
    }

    /* ensure capacity for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap == 0 ? 1 : cap;
        while (new_cap < (size_t)n + 1) {
            new_cap <<= 1;
        }
        A    = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
        if (!A || !pref) { /* allocation failed – return 0 (unlikely) */
            return (seq_t)0;
        }
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t k = sz; k <= (size_t)n; ++k) {
        seq_t inner = (seq_t)0;
        size_t i = 1;
        /* harmonic loop: process ranges with equal floor(k / i) */
        while (i < k) {
            size_t q   = k / i;
            size_t nxt = k / q;
            if (nxt > k - 1) nxt = k - 1;
            seq_t rangeSum = pref[nxt] - pref[i - 1];
            inner += (seq_t)q * rangeSum;
            i = nxt + 1;
        }
        seq_t ak = (seq_t)1 + inner;  /* a(k) = 1 + inner sum */
        A[k] = ak;
        pref[k] = pref[k - 1] + ak;
        sz++;
    }

    return A[(size_t)n];
}
