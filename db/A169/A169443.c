/*
 * A169443  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169443: a(0)=1.
   For n>0:
     a(n) = 44 * sum_{i=1}^{31} a(n-i) - 990 * a(n-32) + c(n)
   where c(n)=2 for 1≤n≤31, c(32)=1, otherwise c(n)=0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *buf = NULL;   /* array of values */
    static size_t capacity = 0; /* allocated size */
    static idx_t max_comp = 0;  /* highest index already computed */

    /* initial allocation and a(0) */
    if (!buf) {
        capacity = 64;                   /* start with 64 slots */
        buf = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!buf)
            return (seq_t)0; /* memory allocation failed */
        buf[0] = (seq_t)1;               /* a(0) */
        max_comp = 0;
    }

    /* enlarge buffer if needed */
    if ((size_t)n > capacity-1) {
        size_t newcap = capacity ? capacity : 64;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* memory allocation failed */
        buf = tmp;
        capacity = newcap;
    }

    /* compute missing terms up to n */
    if (n > max_comp) {
        for (size_t k = max_comp + 1; k <= (size_t)n; ++k) {
            /* compute sum of up to 31 previous terms */
            seq_t sumA = (seq_t)0;
            for (size_t d = 1; d <= 31 && k >= d; ++d)
                sumA += buf[k - d];

            /* coefficient c(k) from the numerator */
            seq_t c = (seq_t)0;
            if (k >= 1 && k <= 31)
                c = (seq_t)2;
            else if (k == 32)
                c = (seq_t)1;

            /* compute a(k) via recurrence */
            seq_t term = c + sumA * (seq_t)44;
            if (k >= 32)
                term = term - buf[k - 32] * (seq_t)990;
            buf[k] = term;
        }
        max_comp = n;
    }

    return buf[(size_t)n];
}
