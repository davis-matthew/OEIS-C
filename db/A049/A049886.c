/*
 * A049886  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A049886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049886
   a(1) = a(2) = a(3) = 1
   For n ≥ 4:
       let x = n – 1
       let p be the unique integer with 2^p < x ≤ 2^(p+1)
       let m = x – 2^p
       then a(n) = (a(1)+…+a(n‑1)) – a(m)   */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of the sequence and its cumulative sums   */
    static seq_t *A = NULL;          /* a[1] … a[n]   */
    static seq_t *S = NULL;          /* S[i] = a[1]+…+a[i]   */
    static size_t cap = 0;           /* allocated size   */
    static idx_t  computed = 0;      /* highest index computed */

    if (n == 0)  /* out of range, defined as 0 */
        return (seq_t)0;

    /* enlarge cache if necessary   */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 4) newcap = 4;
        seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        seq_t *tmpS = (seq_t *)realloc(S, newcap * sizeof(seq_t));
        if (!tmpA || !tmpS)   /* allocation failure */
            return (seq_t)0;
        A = tmpA;  S = tmpS;  cap = newcap;
    }

    /* initialise the unused parts        */
    if (cap == 0) {
        A[0] = (seq_t)0;
        S[0] = (seq_t)0;
    }

    /* compute terms up to n             */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 3) {                /* initial values */
            A[i] = (seq_t)1;
            S[i] = S[i - 1] + (seq_t)1;
            continue;
        }
        /* find largest power of two < (i-1)   */
        idx_t x = i - 1;
        idx_t power = 1;
        while ((power << 1) < x)
            power <<= 1;
        idx_t m = x - power;             /* 1 ≤ m < i */

        seq_t a_m   = A[m];
        seq_t sum_i = S[i - 1];           /* a(1)+…+a(i-1)   */

        A[i] = sum_i - a_m;
        S[i] = sum_i + A[i];
    }

    computed = n;
    return A[n];
}
