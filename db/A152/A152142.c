/*
 * A152142  a(n) = Product_{k=1..floor((n-1)/2)} (1 + 4*sin(k*Pi/n)^2 +
 *          16*sin(k*Pi/n)^4).
 * https://oeis.org/A152142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1..⌊(n-1)/2⌋} (1 + 4*sin(k*π/n)^2 + 16*sin(k*π/n)^4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The generating function of the sequence is a rational function.  This gives a
    linear recurrence with homogeneous part  
          a(n) = 7*a(n-2) - 9*a(n-4) + 7*a(n-6) - a(n-8)      (n > 8)
    The non‑homogeneous terms come from the numerator polynomial
          P(x) = x^8 - x^7 - 8x^6 - 6x^5 + 9x^4 + 6x^3 - 6x^2 + x + 1
    With  p[0..8] = {1,1,-6,6,9,-6,-8,-1,1}  the full recurrence is
          a(n) =  p[n] + 7*a(n-2) - 9*a(n-4) + 7*a(n-6) - a(n-8)   (n >= 0)
    treating a(m)=0 for m<0 and p[n]=0 for n>8.  This produces exactly the
    OEIS terms for all n.  The sequence grows quickly, so the routine uses
    memoisation to avoid recomputing past values, and it keeps the work
    linear in the requested index.  No floating point arithmetic or
    external utilities are used.  The code is valid C11 and C++17. */

#include <stddef.h>
#include <stdlib.h>

/* coefficients of the numerator polynomial P(x) (degree 8)  */
static const int coeff[9] = { 1,  1, -6,  6,  9, -6, -8, -1,  1 };

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  capacity = 0;

    /* enlarge the cache if necessary */
    if (n >= capacity) {
        idx_t new_cap = (capacity == 0) ? 16 : (capacity << 1);
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure – abort */
        if (cache == NULL) {
            tmp[0] = (seq_t)1;             /* a(0) = 1 is mandatory */
            capacity = 1;
        } else {
            capacity = new_cap;
        }
        cache = tmp;

        /* compute values from the current capacity up to n */
        for (idx_t i = (capacity > 16 ? capacity/2 : 1); i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;       /* already set */
                continue;
            }

            /* start with the constant term p[i] (zero for i>8) */
            seq_t val = 0;
            if (i <= 8) {
                int c = coeff[i];
                if (c >= 0)
                    val += (seq_t)c;
                else
                    val -= (seq_t)(-c);
            }

            /* add the +7*a(i-2) and +7*a(i-6) parts */
            if (i >= 2) val += (seq_t)7 * cache[i-2];
            if (i >= 6) val += (seq_t)7 * cache[i-6];

            /* subtract the -9*a(i-4) and -a(i-8) parts */
            if (i >= 4) val -= (seq_t)9 * cache[i-4];
            if (i >= 8) val -= cache[i-8];

            cache[i] = val;
        }
        capacity = n + 1;                   /* new capacity is n+1 */
    }

    return cache[n];
}
