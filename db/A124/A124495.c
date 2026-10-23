/*
 * A124495  G.f.: A(x) = 1/[1-x - Sum_{n>=1} A001147(n)*x^(2n) ] where
 *          A001147(n) = (2n)!/(n!*2^n) is the double factorials.
 * https://oeis.org/A124495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0 a(n)=a(n-1)+Σ_{k=1}^{⌊n/2⌋} A001147(k)*a(n-2k) ,
 *            where A001147(k) = (2k)!/(k!*2^k) = (2k–1)!! (double factorial
 *            of odd numbers)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /*--- memoisation for a(n) -------------------------------------------*/
    static seq_t *arr = NULL;     /* array of a(0) … a(last) */
    static size_t cap = 0;        /* allocated capacity of arr      */
    static idx_t last = 0;        /* highest index already computed */

    /*--- cache for A001147(k) = (2k-1)!! ------------------------------*/
    static seq_t *dfarr = NULL;   /* dfarr[k] = (2k-1)!! (k≥0)      */
    static size_t dcap = 0;       /* allocated capacity of dfarr    */
    static idx_t max_c = 0;       /* highest k fully cached         */

    /*--- ensure arr has space up to n --------------------------------*/
    if (arr == NULL) {                    /* first call */
        size_t need = (size_t)(n + 1) < 64 ? 64 : (size_t)(n + 1);
        arr = seq_alloc_array(need);
        cap = need;
        arr[0] = (seq_t)1;                /* a(0) = 1          */
        last = 0;
    } else if ((size_t)(n + 1) > cap) {   /* enlarge if needed */
        size_t need = (size_t)(n + 1) < 64 ? 64 : (size_t)(n + 1);
        seq_t *tmp = seq_alloc_array(need);
        for (idx_t i = 0; i <= last; ++i)
            tmp[i] = arr[i];
        free(arr);
        arr = tmp;
        cap = need;
    }

    /*--- ensure double‑factorial cache covers up to ⌊n/2⌋ ------------*/
    idx_t need_c = n / 2;
    if (dfarr == NULL) {                    /* first call */
        size_t dneed = (size_t)(need_c + 1);
        dfarr = seq_alloc_array(dneed);
        dcap = dneed;
        dfarr[0] = (seq_t)1;                /* (2*0-1)!! = 1 */
        for (idx_t k = 1; k <= need_c; ++k)
            dfarr[k] = (seq_t)((unsigned __int128)dfarr[k-1]
                               * (unsigned __int128)(2 * k - 1));
        max_c = need_c;
    } else if (need_c > max_c) {            /* grow cache */
        size_t dneed = (size_t)(need_c + 1);
        seq_t *tmp = seq_alloc_array(dneed);
        for (idx_t i = 0; i <= max_c; ++i)
            tmp[i] = dfarr[i];
        free(dfarr);
        dfarr = tmp;
        dcap = dneed;
        for (idx_t k = max_c + 1; k <= need_c; ++k)
            dfarr[k] = (seq_t)((unsigned __int128)dfarr[k-1]
                               * (unsigned __int128)(2 * k - 1));
        max_c = need_c;
    }

    /*--- compute a(n) up to required index ---------------------------*/
    while (last < n) {
        ++last;
        /* use 128‑bit arithmetic for the partial sum to avoid overflow */
        unsigned __int128 temp = (unsigned __int128)arr[last - 1];
        for (idx_t k = 1; k <= last / 2; ++k)
            temp += (unsigned __int128)dfarr[k] *
                    (unsigned __int128)arr[last - 2 * k];
        arr[last] = (seq_t)temp;
    }

    return arr[n];
}
