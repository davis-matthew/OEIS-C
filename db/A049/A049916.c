/*
 * A049916  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A049916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049916
 *
 * Recurrence:
 *   a(1) = 1,  a(2) = 3,  a(3) = 1
 *   For n >= 4:
 *     Let p be the unique integer with 2^p < n-1 <= 2^(p+1)
 *     Let m = 2*n - 3 - 2^(p+1)
 *     a(n) = a(1)+a(2)+...+a(n-1) - a(m)
 *    i.e. a(n) = S(n-1) - a(m), where S(k) = sum_{i=1..k} a(i)
 *
 * The implementation keeps a static memoized table of a(n) and of the
 * cumulative sums S(n).  When a new n is requested the table is
 * extended and terms are filled in increasing order, so each term
 * requires only O(1) work.  This O(n) algorithm is fast enough up to
 * thousands of terms for all supported integer sizes.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches: a[n] and S[n] = sum_{i=1}^n a[i] */
    static seq_t *A       = NULL;
    static seq_t *S       = NULL;
    static idx_t  cache_sz = 0;                /* number of valid entries (includes index 0) */

    if (n < 1)
        return (seq_t)0;                       /* undefined, but avoid UB */

    /* Grow cache to hold indices up to n (array size n+1) */
    if (cache_sz <= n)
    {
        idx_t new_size = n + 1;
        seq_t *A_new = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        seq_t *S_new = (seq_t *)realloc(S, new_size * sizeof(seq_t));
        if (!A_new || !S_new)  /* allocation failure → terminate */
        {
            free(A_new);
            free(S_new);
            abort();
        }

        /* Initialize new entries */
        for (idx_t i = cache_sz; i <= n; ++i)
        {
            seq_t val;
            if (i == 0)
            {
                val = (seq_t)0;
            }
            else if (i == 1)
            {
                val = (seq_t)1;            /* a(1) */
            }
            else if (i == 2)
            {
                val = (seq_t)3;            /* a(2) */
            }
            else if (i == 3)
            {
                val = (seq_t)1;            /* a(3) */
            }
            else    /* i >= 4 */
            {
                /* Compute p:   floor(log2(i-2))   (works for all i>=4) */
                uint64_t t = (uint64_t)(i - 2);
                int p = 0;
                while ((uint64_t)1 << (p + 1) <= t)
                    ++p;

                /* m = 2*i - 3 - 2^(p+1) */
                seq_t pow2 = (seq_t)1 << (p + 1);
                idx_t pow2i = (idx_t)pow2;
                idx_t m = 2 * i - 3 - pow2i;

                /* a(i) = S(i-1) - a(m) */
                val = S_new[i - 1] - A_new[m];
            }

            A_new[i] = val;
            S_new[i] = (i == 0) ? (seq_t)0 : (S_new[i - 1] + A_new[i]);
        }

        A = A_new;
        S = S_new;
        cache_sz = new_size;
    }

    return A[n];
}
