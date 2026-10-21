/*
 * A087689  Numerators of successive partial sums of sum(1/(2^n-1)).
 * https://oeis.org/A087689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the partial sum ∑_{k=1..n} 1/(2^k − 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Euclidean algorithm for seq_t (works for built‑in and user‑defined types) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static storage for computed terms */
    static seq_t *num = NULL;                 /* numerators  (0‑based) */
    static seq_t *den = NULL;                 /* denominators */
    static idx_t   max_n = 0;                 /* number of terms stored */
    static idx_t   cap   = 0;                 /* current array capacity */
    static seq_t   pow2  = (seq_t)1;           /* 2^current index (prev) */

    if (n == 0)                 /* not required, but keeps safety */
        return 0;

    if (n > max_n)
    {
        /* Ensure sufficient capacity */
        if (cap == 0) cap = 1;
        while (cap < n) cap <<= 1;

        if (num == NULL || den == NULL)
        {
            num = (seq_t*)realloc(num, cap * sizeof(seq_t));
            den = (seq_t*)realloc(den, cap * sizeof(seq_t));
        }
        else
        {
            seq_t *tmp = (seq_t*)realloc(num, cap * sizeof(seq_t));
            if (tmp) num = tmp;
            tmp = (seq_t*)realloc(den, cap * sizeof(seq_t));
            if (tmp) den = tmp;
        }

        /* Compute missing terms up to n */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            pow2 = pow2 * (seq_t)2;            /* 2^i */
            seq_t den_term = pow2 - (seq_t)1;  /* 2^i − 1 */

            if (i == 1)                       /* first term */
            {
                num[0] = (seq_t)1;
                den[0] = (seq_t)1;
            }
            else
            {
                seq_t prev_num = num[i - 2];
                seq_t prev_den = den[i - 2];

                seq_t new_num = prev_num * den_term + prev_den;
                seq_t new_den = prev_den * den_term;
                seq_t g = gcd_seq_t(new_num, new_den);
                new_num = new_num / g;
                new_den = new_den / g;

                num[i - 1] = new_num;
                den[i - 1] = new_den;
            }
        }
        max_n = n;
    }

    /* Return the numerator of the n‑th term (1‑based index) */
    return num[n - 1];
}
