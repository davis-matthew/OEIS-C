/*
 * A102718  a(n) = a(n-1) + (sum of the last two digits of the sequence so
 *          far); a(0)=0; a(1)=1.
 * https://oeis.org/A102718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n≥2, a(n) = a(n-1) + (sum of the two last decimal digits of the
   concatenated sequence a(0)…a(n-1)). */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *A      = NULL;   /* stored terms a(0)…a(cap-1)          */
    static seq_t *SUF    = NULL;   /* stored suffixes (last 2 decimal digits) of string a(0)…a(i) */
    static idx_t cap     = 0;      /* current allocated capacity          */
    static idx_t nextIdx = 0;      /* next index that needs to be computed */

    /* 1. Allocate or grow array to size n+1 if necessary */
    if (n >= cap) {
        idx_t newCap = n + 1;
        seq_t *Anew  = (seq_t *)realloc(A, newCap * sizeof(seq_t));
        seq_t *SUFnew= (seq_t *)realloc(SUF, newCap * sizeof(seq_t));
        if (!Anew || !SUFnew) return 0; /* allocation failure -- unlikely */

        /* Construct the newly created elements (C++ only) */
        for (idx_t i = cap; i < newCap; ++i) {
#ifdef __cplusplus
            ::new((void *)&Anew[i]) seq_t();
            ::new((void *)&SUFnew[i]) seq_t();
#endif
        }

        A   = Anew;
        SUF = SUFnew;
        cap = newCap;
    }

    /* 2. Initialise base cases if they are not already present */
    while (nextIdx <= 1) {
        if (nextIdx == 0 && cap >= 1) {
            A[0]   = (seq_t)0;
            SUF[0] = (seq_t)0;          /* suffix of the string "0" is 0 */
        } else if (nextIdx == 1 && cap >= 2) {
            A[1]   = (seq_t)1;
            SUF[1] = (seq_t)1;          /* suffix of the string "01" is 1 */
        }
        ++nextIdx;
    }

    /* 3. Compute missing terms up to n */
    for (idx_t i = nextIdx; i <= n; ++i) {
        /* sum of the last two decimal digits of SUF[i-1] */
        seq_t s   = SUF[i - 1];
        seq_t add = (s / (seq_t)10) + (s % (seq_t)10);

        /* new term */
        seq_t ai = A[i - 1] + add;
        A[i] = ai;

        /* update the suffix for the next step */
        seq_t new_s;
        if (ai < (seq_t)10) {
            new_s = (s * (seq_t)10 + ai) % (seq_t)100;
        } else {
            /* when ai has at least two digits, its lower two digits
               become the new suffix, because the high part is a multiple of 100 */
            new_s = ai % (seq_t)100;
        }
        SUF[i] = new_s;
    }

    nextIdx = n + 1;
    return A[n];
}
