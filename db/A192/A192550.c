/*
 * A192550  a(n) = sum(stirling2(n+1,k+1)*k!^2,k=0..n).
 * https://oeis.org/A192550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} Stirling2(n+1,k+1)*k!^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* memcpy, memset */

seq_t a(idx_t n)
{
    /* Static tables holding the current Stirling-row values
       and the already computed sequence values.              */
    static seq_t *T = NULL;          /* T[j] = S(current_n,j)*(j-1)!^2 */
    static size_t T_cap = 0;
    static seq_t *a_vals = NULL;     /* a_vals[i] = a(i) */
    static size_t a_cap = 0;
    static size_t cur_n = 0;         /* maximum n processed so far */

    /* first call – initialise the tables   */
    if (T == NULL) {
        /* At least capacity 1 to hold T[0] */
        T = NULL;
#ifdef __cplusplus
        T = new seq_t[1];
#else
        T = (seq_t*)calloc(1, sizeof(seq_t));
#endif
        if (T == NULL) abort();
        T_cap = 1;
        T[0] = (seq_t)0;

        a_vals = NULL;
#ifdef __cplusplus
        a_vals = new seq_t[1];
#else
        a_vals = (seq_t*)calloc(1, sizeof(seq_t));
#endif
        if (a_vals == NULL) abort();
        a_cap = 1;
        a_vals[0] = (seq_t)1;   /* a(0) = 1 */
        cur_n = 0;
    }

    /* grow the tables if needed   */
    if ((size_t)n >= a_cap) {
        size_t new_cap = (size_t)n + 1;

        /* grow T  */
        if (T_cap < new_cap) {
            seq_t *newT = NULL;
#ifdef __cplusplus
            newT = new seq_t[new_cap];
            /* copy old data */
            for (size_t i = 0; i < T_cap; ++i) newT[i] = T[i];
            delete[] T;
#else
            newT = (seq_t*)realloc(T, new_cap * sizeof(seq_t));
            if (newT == NULL) abort();
            memset(newT + T_cap, 0, (new_cap - T_cap) * sizeof(seq_t));
            T = newT;
#endif
            T = newT;
            T_cap = new_cap;
        }

        /* grow a_vals  */
        if (a_cap < new_cap) {
            seq_t *newA = NULL;
#ifdef __cplusplus
            newA = new seq_t[new_cap];
            for (size_t i = 0; i < a_cap; ++i) newA[i] = a_vals[i];
            delete[] a_vals;
#else
            newA = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
            if (newA == NULL) abort();
            memset(newA + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
            a_vals = newA;
#endif
            a_vals = newA;
            a_cap = new_cap;
        }

        /* If we haven't yet computed a(1), do it now   */
        if (cur_n == 0 && n >= 1) {
            T[1] = (seq_t)1;            /* S(1,1)=1 */
            seq_t sum = (seq_t)0;
            sum += (seq_t)1 * (seq_t)2 * T[1];   /* 1*2*T1 */
            a_vals[1] = sum;            /* a(1)=2 */
            cur_n = 1;
        }

        /* Compute further rows, if any   */
        for (size_t m = cur_n + 1; m <= (size_t)n; ++m) {
            /* make sure T[m] exists and is zero before the update */
            T[m] = (seq_t)0;
            /* update T in reverse order to keep old values */
            for (size_t j = m; j > 0; --j) {
                seq_t old_Tj   = T[j];
                seq_t old_Tjm1 = T[j - 1];
                seq_t jm1 = (seq_t)(j - 1);
                seq_t sq = jm1 * jm1;            /* (j-1)^2   */
                seq_t new_val = (seq_t)j * old_Tj + sq * old_Tjm1;
                T[j] = new_val;
            }
            /* compute a(m) from the new T row   */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= m; ++j) {
                seq_t prod = (seq_t)j * (seq_t)(j + 1);
                sum += prod * T[j];
            }
            a_vals[m] = sum;
        }
        cur_n = n;
    }
    return a_vals[n];
}
