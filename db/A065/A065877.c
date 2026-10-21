/*
 * A065877  Non-Niven (or non-Harshad) numbers: numbers which are not a
 *          multiple of the sum of their digits.
 * https://oeis.org/A065877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth non‑Harshad (non‑Niven) number.
   Starting from the first term a(1)=11, it scans successive integers,
   keeps those which are not divisible by their digit sum,
   and caches the results for fast repeated access. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                     /* offset is 1 – undefined for n<1 */
        return (seq_t)0;

    /* Static cache for already computed terms */
    static seq_t *cache = NULL;     /* array of computed terms              */
    static size_t  cap   = 0;       /* allocated capacity (in elements)     */
    static size_t  len   = 0;       /* number of terms stored so far        */
    static idx_t   cand  = 1;       /* next integer to test                  */

    /* Allocate more space if needed */
    if (len < (size_t)n) {
        if (cap < (size_t)n) {
            size_t newcap = cap ? cap : 64;
            while (newcap < (size_t)n)
                newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)         /* allocation failure – return 0 sentinel */
                return (seq_t)0;
            cache = tmp;
            cap   = newcap;
        }

        /* Generate terms until we have n of them */
        while (len < (size_t)n) {
            seq_t x  = (seq_t)cand;
            seq_t sum = 0;
            seq_t t   = x;

            /* digit sum of x */
            while (t != (seq_t)0) {
                sum += t % (seq_t)10;
                t   /= (seq_t)10;
            }

            /* store if x is NOT a Harshad number */
            if (sum != (seq_t)0 && x % sum != (seq_t)0) {
                cache[len++] = x;
            }
            ++cand;
        }
    }

    /* cache is 0‑indexed: a(1) is cache[0] */
    return cache[n - 1];
}
