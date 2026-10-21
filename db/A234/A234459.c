/*
 * A234459  Real part of the product of all the integer complex numbers in the
 *          square [1,1] to [n,n].
 * https://oeis.org/A234459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (∏_{x<y≤n} (x² + y²)) · n! · Re( (1+i)ⁿ · i^{n(n‑1)/2} )
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 5, i128 n <= 7,
 *            big any n
 * Helpers:   cmplx_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The real part of the product of all integer complex numbers in the square
   [1,1] to [n,n] (with offset 1).  The recurrence below computes each term
   from the previous one and memoises intermediate results for arbitrary
   call order. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmplx_mul.h"

seq_t a(idx_t n)
{
    /* Memoisation tables.  Index 0 is unused; data for n ≥ 1 are stored at
       indices 1..max_pre. */
    static seq_t *real_part  = NULL;   /* (1+i)ⁿ real part */
    static seq_t *imag_part  = NULL;   /* (1+i)ⁿ imaginary part */
    static seq_t *fact_prod  = NULL;   /* n! · ∏_{x<y≤n} (x² + y²) */
    static idx_t  max_pre    = 0;      /* largest n computed so far */
    static size_t cap        = 0;      /* current array capacity */

    if (n < 1) return 0;  /* undefined for n<1; return 0 */

    /* Grow arrays if needed */
    if (n > max_pre) {
        size_t new_cap = (cap == 0) ? 16 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        /* Allocate or reallocate homogeneously using calloc for zero init. */
        if (cap == 0) {
            real_part = (seq_t *)calloc(new_cap, sizeof(seq_t));
            imag_part = (seq_t *)calloc(new_cap, sizeof(seq_t));
            fact_prod = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            real_part = (seq_t *)realloc(real_part, new_cap * sizeof(seq_t));
            imag_part = (seq_t *)realloc(imag_part, new_cap * sizeof(seq_t));
            fact_prod = (seq_t *)realloc(fact_prod, new_cap * sizeof(seq_t));
        }
        /* Initialise any new entries to zero. */
        for (size_t i = cap + 1; i < new_cap; ++i) {
            real_part[i] = 0;
            imag_part[i] = 0;
            fact_prod[i] = 0;
        }
        cap = new_cap;

        /* Build values from max_pre+1 up to n */
        for (idx_t k = max_pre + 1; k <= n; ++k) {
            /* 1. Compute (1+i)^k via recurrence (prev * (1+i)) */
            if (k == 1) {
                real_part[1] = 1;   /* (1+i)^1 = 1 + i */
                imag_part[1] = 1;
            } else {
                seq_t prev_r = real_part[k - 1];
                seq_t prev_i = imag_part[k - 1];
                real_part[k] = prev_r - prev_i; /* (1+i)*(prev_r + i*prev_i) */
                imag_part[k] = prev_r + prev_i;
            }

            /* 2. Compute fact_prod[k] = k! · ∏_{x<y≤k} (x² + y²) */
            if (k == 1) {
                fact_prod[1] = 1;   /* 1! · (empty product) = 1 */
            } else {
                seq_t prod = 1;               /* ∏_{x=1}^{k-1} (x² + k²) */
                seq_t ksq = (seq_t)k * (seq_t)k;
                for (idx_t x = 1; x < k; ++x) {
                    seq_t xsq = (seq_t)x * (seq_t)x;
                    seq_t term = xsq + ksq;
                    prod = prod * term;
                }
                /* fact_prod[k] = k * fact_prod[k-1] * prod */
                fact_prod[k] = fact_prod[k - 1] * prod;
                fact_prod[k] = fact_prod[k] * (seq_t)k;
            }
        }

        max_pre = n;
    }

    /* Compute the real part of (1+i)^n · i^{C}  where C = n(n‑1)/2. */
    idx_t C = (n * (n - 1)) / 2;
    int cmod = (int)(C & 3);   /* C modulo 4 */

    seq_t qreal;
    switch (cmod) {
        case 0: qreal = real_part[n];     break;
        case 1: qreal = -imag_part[n];    break;
        case 2: qreal = -real_part[n];    break;
        case 3: qreal = imag_part[n];     break;
        default: qreal = 0; /* never reached */ break;
    }

    return fact_prod[n] * qreal;
}
