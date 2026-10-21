/*
 * A221159  a(n) = Sum_{i=0..n} Stirling2(n,i)*2^(3i).
 * https://oeis.org/A221159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} Stirling2(n,i)*8^i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the nth term of sequence A221159.
   It keeps a static cache of already computed values, growing the
   cache as needed.  The implementation uses the recurrence

       a(0) = 1
       a(n) = 8 * sum_{k=0}^{n-1} C(n-1, k) * a(k)     (n ≥ 1)

   Internally it maintains the array s[k] = S(n,k) * 8^k
   for the current n, and updates it in O(n) time per n, giving
   an overall O(n²) algorithm which easily meets the limits for
   n ≤ 10 000.  All arithmetic is performed on the type seq_t
   as required by the problem statement. */
seq_t a(idx_t n)
{
    static idx_t  computed = 0;   /* highest n computed so far          */
    static idx_t  capacity = 0;   /* size of allocated arrays (≥ n+1)  */
    static seq_t *S = NULL;       /* s[k] = S(n,k) * 8^k  for current n  */
    static seq_t *A = NULL;       /* a values for each n                */

    if (!capacity)                 /* first call: initialise cache  */
    {
        capacity = 16;
        S = (seq_t *)calloc(capacity, sizeof(seq_t));
        A = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!S || !A) return 0; /* out‑of‑memory: return 0 as a last resort */

        S[0] = (seq_t)1;    /* S(0,0)*8^0 = 1   */
        A[0] = (seq_t)1;    /* a(0) = 1         */
        computed = 0;
    }

    if (n > computed)             /* compute missing terms          */
    {
        /* grow arrays if necessary                 */
        if (capacity <= (idx_t)n)
        {
            idx_t newcap = capacity;
            while (newcap <= (idx_t)n) newcap <<= 1;

            seq_t *tmpS = (seq_t *)realloc(S, newcap * sizeof(seq_t));
            seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
            if (!tmpS || !tmpA) return 0;
            S = tmpS;
            A = tmpA;

            /* initialise the newly allocated slots to zero */
            for (idx_t i = capacity; i < newcap; ++i)
            {
                S[i] = 0;
                A[i] = 0;
            }
            capacity = newcap;
        }

        /* compute terms from computed+1 up to n        */
        for (idx_t cur = computed + 1; cur <= n; ++cur)
        {
            /* update s[k] = S(cur,k)*8^k using the recurrence
               s_k = k * s_k + 8 * s_{k-1}          (k≥1)   */
            for (idx_t k = cur; k > 0; --k)
            {
                seq_t new_val = S[k] * (seq_t)k + S[k-1] * (seq_t)8;
                S[k] = new_val;
            }
            /* s_0 is always 0 for n≥1                */
            S[0] = 0;

            /* sum the entries to obtain a(cur)        */
            seq_t sum = 0;
            for (idx_t k = 1; k <= cur; ++k)
                sum += S[k];

            A[cur] = sum;
        }
        computed = n;
    }

    return A[n];
}
