/*
 * A160098  Positive numbers y such that y^2 is of the form x^2+(x+601)^2 with
 *          integer x.
 * https://oeis.org/A160098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 67, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160098: a(n) = 6*a(n-3) - a(n-6) for n > 6
   with a(1)=425, a(2)=601, a(3)=1261, a(4)=1289, a(5)=3005, a(6)=7141. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;     /* 1‑based array of computed terms */
    static idx_t size = 0;        /* number of terms already stored */
    static size_t cap  = 0;       /* allocation capacity (>= size+1) */

    if (n == 0)            /* safety – the sequence is only defined for n ≥ 1 */
        return (seq_t)0;

    /* enlarge buffer if necessary */
    if (n > size) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (new_cap > cap) {
            buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* initialise the first six values if not yet done */
        if (size < 1) buf[1] = 425;
        if (size < 2) buf[2] = 601;
        if (size < 3) buf[3] = 1261;
        if (size < 4) buf[4] = 1289;
        if (size < 5) buf[5] = 3005;
        if (size < 6) buf[6] = 7141;

        /* iterate forward to compute missing terms */
        for (idx_t i = size + 1; i <= n; ++i) {
            if (i <= 6)           /* already initialised */
                continue;
            buf[i] = 6 * buf[i - 3] - buf[i - 6];
        }
        size = n;
    }

    return buf[n];
}
