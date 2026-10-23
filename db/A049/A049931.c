/*
 * A049931  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 4.
 * https://oeis.org/A049931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=4; for n>=4, a(n) = (a(1)+...+a(n-1)) –
 *            a(m), where m = 2*n - 2 - 2^(p+1) and 2^p < n-1 <= 2^(p+1)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* floor(log2(v)) for unsigned 64‑bit integer v >= 1 */
static inline idx_t log2_floor(idx_t v)
{
    idx_t p = 0;
    while (v > 1) {
        v >>= 1;
        ++p;
    }
    return p;
}

seq_t a(idx_t n)
{
    static seq_t *a_val = NULL;
    static seq_t *pref   = NULL;     /* prefix sums */
    static size_t cap    = 0;        /* allocated capacity (indexable up to cap-1) */
    static idx_t last    = 0;        /* highest index computed so far */

    if (n == 0)
        return (seq_t)0;             /* unused, but safe */

    if (n > last) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            a_val = (seq_t *)realloc(a_val, new_cap * sizeof(seq_t));
            pref  = (seq_t *)realloc(pref,  new_cap * sizeof(seq_t));
            if (!a_val || !pref)
                abort();            /* out of memory */

            cap = new_cap;
        }
        if (last == 0) {              /* initialise the zero index */
            a_val[0] = (seq_t)0;
            pref[0]  = (seq_t)0;
        }

        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1)
                a_val[i] = (seq_t)1;
            else if (i == 2)
                a_val[i] = (seq_t)3;
            else if (i == 3)
                a_val[i] = (seq_t)4;
            else {
                idx_t k   = i - 1;                       /* k = n-1 */
                idx_t p   = log2_floor(k - 1);           /* p = floor(log2(n-2)) */
                idx_t pow = ((idx_t)1) << (p + 1);        /* 2^(p+1) */
                idx_t m   = 2 * i - 2 - pow;             /* m = 2n-2-2^(p+1) */
                a_val[i] = pref[i - 1] - a_val[m];
            }
            pref[i] = pref[i - 1] + a_val[i];
        }
        last = n;
    }
    return a_val[n];
}
