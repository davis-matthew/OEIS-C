/*
 * A184485  Upper s-Wythoff sequence, where s(n)=3n-2. Complement of A184484.
 * https://oeis.org/A184485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lower(n) + (3 n – 2)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where lower(n) is the smallest natural number that is not in any */
/*  previous lower or upper element of the sequence.  This implements an */
/*  incremental construction of the upper s‑Wythoff sequence with s(n)=3n–2. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Dynamic array that marks numbers already occupied by a lower or an
       upper value.  The array grows as needed while the index grows. */
    size_t cap = 16;
    uint8_t *used = (uint8_t*)calloc(cap, sizeof(uint8_t));
    if (!used) abort();

    idx_t next = 1;          /* candidate for lower(n)              */
    seq_t  result = 0;       /* a(n) to be returned                 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Ensure the array is large enough to index 'next'. */
        while ((size_t)next >= cap) {
            size_t newcap = cap ? cap * 2 : 8;
            while ((size_t)next >= newcap) newcap <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(used, newcap);
            if (!tmp) { free(used); abort(); }
            memset(tmp + cap, 0, newcap - cap);
            used = tmp;
            cap = newcap;
        }

        /* Advance to the next unused integer. */
        while (used[(size_t)next]) ++next;

        /* Mark this integer as used – it becomes lower(i). */
        used[(size_t)next] = 1;
        idx_t lower = next;

        /* Compute the upper element for this pair. */
        seq_t upper = (seq_t)lower + (seq_t)(3 * i - 2);
        size_t uval = (size_t)upper;

        /* Ensure the array can index the 'upper' value. */
        while (uval >= cap) {
            size_t newcap = cap ? cap * 2 : 8;
            while (uval >= newcap) newcap <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(used, newcap);
            if (!tmp) { free(used); abort(); }
            memset(tmp + cap, 0, newcap - cap);
            used = tmp;
            cap = newcap;
        }

        /* Mark the upper value as used. */
        used[uval] = 1;

        /* The required value for this n. */
        result = upper;
    }

    free(used);
    return result;
}
