/*
 * A120144  a(n) = 19 + floor( Sum_{j=1..n-1} a(j) / 2 ).
 * https://oeis.org/A120144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 48, u64 n <= 103, u128 n <= 211,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A120144
 *  Recurrence:  a(1) = 19,
 *                a(n) = 19 + floor( sum_{j=1}^{n-1} a(j) / 2 )  for n ≥ 2
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset starts at 1 */

    static idx_t    sz = 0;                     /* number of terms cached   */
    static idx_t    cap = 0;                    /* capacity of the array    */
    static seq_t  *buf = NULL;                  /* cache of computed terms  */
    /* decide whether to use 128‑bit accumulation */
    static const int use128 = (sizeof(seq_t) <= 8) ? 1 : 0;

    /* values for running sums */
    static __uint128_t sum128 = 0;              /* used when seq_t ≤ 64 bits */
    static seq_t      sum64 = 0;                /* used otherwise (big int) */

    /* Return cached value if already computed */
    if (buf != NULL && n <= sz)
        return buf[(size_t)(n - 1)];

    /* Initialise buffer on first call */
    if (buf == NULL) {
        cap = (n > 1 ? n : 1);
        buf = (seq_t*)malloc((size_t)cap * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0;       /* allocation failed */
        sz  = 0;
    }

    /* Grow buffer if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* reallocation failed */
        buf = tmp;
        cap = (idx_t)new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = sz; i < n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)19;
        } else if (use128) {
            __uint128_t v128 = (unsigned __int128)19 + (sum128 >> 1);
            val = (seq_t)v128;
        } else {
            val = (seq_t)19 + (sum64 / 2);
        }

        buf[(size_t)i] = val;

        /* update running sum */
        if (use128) {
            sum128 += (unsigned __int128)val;
        } else {
            sum64 += val;
        }
    }
    sz = n;
    return buf[(size_t)(n - 1)];
}
