/*
 * A171847  Total area under all peakless Motzkin paths of length n (n>=0).
 * https://oeis.org/A171847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171847
   Total area under all peakless Motzkin paths of length n (n>=0).
   Recurrence (valid for n>=10):
     (n+2)a(n) +5(-n-1)a(n-1)+(7n-4)a(n-2)+2(-3n+7)a(n-3)
      +(13n-18)a(n-4)+11(-n+3)a(n-5)+(13n-60)a(n-6)+2(-3n+11)a(n-7)
      +(7n-38)a(n-8)+5(-n+7)a(n-9)+(n-8)a(n-10)=0
   Initial values:
     a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=7,
     a(5)=22, a(6)=68, a(7)=198, a(8)=563, a(9)=1578
*/

#include <stdlib.h>

static const seq_t init[10] = {
    0, 0, 0, 2, 7, 22, 68, 198, 563, 1578
};

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *vals = NULL;
    static idx_t size = 0;          /* number of values currently stored */
    static idx_t capacity = 0;      /* allocated size */

    /* Ensure capacity for indices up to n */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *new_vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!new_vals) abort();     /* memory allocation failure */
        vals = new_vals;
        capacity = new_cap;
    }

    /* Initialize the first 10 values if not already done */
    if (size < 10) {
        idx_t first = size;
        idx_t last  = (size < 10) ? 10 : size; /* should be 10 */
        if (last > capacity) last = capacity;
        for (idx_t i = first; i < last; ++i)
            vals[i] = init[i];
        size = (last > 10) ? 10 : last;
    }

    /* Compute missing values up to n */
    for (idx_t k = size; k <= n; ++k) {
        if (k < 10) {
            /* Already initialized from init[] */
            continue;
        }

        /* Compute coefficients for this n=k */
        int coeff1 = -5 * ((int)k + 1);           /* 5*(-k-1) */
        int coeff2 =  7 * ((int)k) - 4;           /* 7k-4 */
        int coeff3 = -6 * ((int)k) + 14;          /* 2*(-3k+7) */
        int coeff4 = 13 * ((int)k) - 18;          /* 13k-18 */
        int coeff5 = -11 * ((int)k) + 33;         /* 11*(-k+3) */
        int coeff6 = 13 * ((int)k) - 60;          /* 13k-60 */
        int coeff7 = -6 * ((int)k) + 22;          /* 2*(-3k+11) */
        int coeff8 =  7 * ((int)k) - 38;          /* 7k-38 */
        int coeff9 = -5 * ((int)k) + 35;          /* 5*(-k+7) */
        int coeff10= ((int)k) - 8;                /* k-8 */

        seq_t sumPos = 0;
        seq_t sumNeg = 0;

        /* Helper to add a term with correct sign */
        #define ADD_TERM(coeff, idx)          \
            if (coeff >= 0)                 \
                sumPos += vals[idx] * (seq_t)(coeff); \
            else                            \
                sumNeg += vals[idx] * (seq_t)(-coeff)

        ADD_TERM(coeff1, k - 1);
        ADD_TERM(coeff2, k - 2);
        ADD_TERM(coeff3, k - 3);
        ADD_TERM(coeff4, k - 4);
        ADD_TERM(coeff5, k - 5);
        ADD_TERM(coeff6, k - 6);
        ADD_TERM(coeff7, k - 7);
        ADD_TERM(coeff8, k - 8);
        ADD_TERM(coeff9, k - 9);
        ADD_TERM(coeff10, k - 10);

        #undef ADD_TERM

        /* numerator = (sumNeg - sumPos) = (k+2) * a(k) */
        seq_t numerator = sumNeg - sumPos;   /* sumNeg >= sumPos by recurrence */
        vals[k] = numerator / ((seq_t)(k + 2));

        /* update size */
        size = k + 1;
    }

    return vals[n];
}
