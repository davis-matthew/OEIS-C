/*
 * A081293  a(n) = A000108(n) + A014137(n).
 * https://oeis.org/A081293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081293:  a(n) = A000108(n) + A014137(n)
 *  The Catalan numbers C(n) satisfy
 *      C(0)=1,
 *      C(n)=C(n-1)*(4*n-2)/(n+1)      (exact division)
 *
 *  The sequence A014137 satisfies
 *      B(0)=1,
 *      B(n)=C(n)+B(n-1)   (n≥1)
 *
 *  Hence
 *      a(n)=C(n)+B(n)=C(n)+C(n)+B(n-1)=2*C(n)+B(n-1).
 *
 *  We memoise C(n) and B(n) in static arrays so that
 *  any call order runs in O(n) time overall.
 */

#include <stdlib.h>
#include <stdbool.h>

/*--- memoised tables -----------------------------------------------*/
static seq_t *C_arr = NULL;   /* Catalan numbers */
static seq_t *B_arr = NULL;   /* A014137 values      */
static size_t  cap   = 0;     /* current capacity     */
static idx_t   last  = 0;     /* highest index cached */
static bool    init  = false; /* tables initialised   */

/*--- public function -----------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise tables on first use */
    if (!init) {
        size_t init_cap = n + 1;
        if (init_cap < 2) init_cap = 2;            /* at least 2 slots */

        C_arr = (seq_t *)malloc(init_cap * sizeof(seq_t));
        B_arr = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!C_arr || !B_arr) {                    /* allocation failed */
            free(C_arr);
            free(B_arr);
            C_arr = B_arr = NULL;
            cap = 0;
            return (seq_t)0;
        }

        C_arr[0] = (seq_t)1;       /* C(0) */
        B_arr[0] = (seq_t)1;       /* B(0) */
        cap = init_cap;
        last = 0;
        init = true;
    }

    /* expand tables if required */
    if (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;   /* ensure space for n */

        seq_t *tmpC = (seq_t *)realloc(C_arr, new_cap * sizeof(seq_t));
        seq_t *tmpB = (seq_t *)realloc(B_arr, new_cap * sizeof(seq_t));
        if (!tmpC || !tmpB) {                      /* allocation failed */
            free(tmpC);
            free(tmpB);
            return (seq_t)0;
        }
        C_arr = tmpC;
        B_arr = tmpB;
        cap   = new_cap;
    }

    /* compute numbers up to n (if not already cached) */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* C(k) = C(k-1) * (4*k-2) / (k+1)  */
        seq_t c = C_arr[k - 1] * (seq_t)(4 * k - 2);
        c = c / (k + 1);

        C_arr[k] = c;
        B_arr[k] = c + B_arr[k - 1];   /* B(k) = C(k) + B(k-1) */
    }
    last = n;

    /* a(n) = C(n) + B(n) */
    return C_arr[n] + B_arr[n];
}
