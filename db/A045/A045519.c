/*
 * A045519  Factorials with initial digit '8'.
 * https://oeis.org/A045519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000142(A045527(n))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th factorial whose first digit is 8. */

#include <math.h>
#include <stddef.h>
#include "../utils/factorial.h"

/* Keep a cache of the indices of the factorials that start with 8.
   We only need to handle up to about 10 000 terms, so a 20 000 entry
   table is ample. */
#define MAX_TERMS 20000
static idx_t found_indices[MAX_TERMS];
static size_t found_count = 0;      /* number of indices stored so far */
static idx_t current_i = 1;         /* last i processed in the scan */
static long double frac = 0.0L;     /* fractional part of log10(i!) */

/* Return the n-th factorial that starts with digit 8. */
seq_t a(idx_t n)
{
    /* Scan factorials until we have found at least n of them. */
    while (found_count < n)
    {
        /* advance to the next integer i */
        current_i += 1;
        /* incremental accumulation of log10(i!) mod 1 */
        frac += log10((long double)current_i);
        frac -= floorl(frac);

        /* leading digit of current_i! */
        long double leading = powl(10.0L, frac);   /* 1 <= leading < 10 */
        int digit = (int)(leading + 1e-12L);       /* safe floor */

        if (digit == 8)
        {
            if (found_count < MAX_TERMS)
                found_indices[found_count] = current_i;
            found_count++;
        }
    }

    /* Retrieve the index of the desired factorial */
    idx_t k = found_indices[n - 1];

    /* Return the factorial of that index */
    return factorial(k);
}
