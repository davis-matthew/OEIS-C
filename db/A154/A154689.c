/*
 * A154689  Numbers n such that sigma_0(n-sigma_0(n))= sigma_0(n).
 * https://oeis.org/A154689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154689: numbers n such that d(n - d(n)) = d(n)  */
/* Requirements: public wrapper a(idx_t n) returning seq_t   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"   /* returns divisor count for uint64_t */
#include "../utils/ensure_capacity.h"    /* expands a dynamic array */

seq_t a(idx_t n)
{
    /* 1‑based index – offset is 1 */
    if (n == 0) return (seq_t)0;

    static seq_t *vals = NULL;      /* stored sequence terms   */
    static size_t sz   = 0;          /* number of terms stored  */
    static size_t cap  = 0;          /* current allocation cap  */
    static uint64_t next = 1;        /* next integer to test    */

    /* generate terms until we have at least 'n' terms */
    while (sz < (size_t)n)
    {
        uint64_t m = next++;

        /* compute d(m) */
        uint64_t d_m = divisor_count_u64(m);

        /* require m > d(m) so that m - d(m) > 0 (unsigned wrap avoided) */
        if (m > d_m)
        {
            uint64_t m_minus = m - d_m;
            /* test the defining property */
            if (divisor_count_u64(m_minus) == d_m)
            {
                /* ensure space for another term */
                ensure_capacity(sz + 1, &cap, (void **)&vals, sizeof(seq_t));
                vals[sz++] = (seq_t)m;
            }
        }
    }

    /* return the nth term (1‑based) */
    return vals[n - 1];
}
