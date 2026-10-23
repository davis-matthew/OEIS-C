/*
 * A108513  Numbers of the form (2^i)*(5^j)*(7^k), with i, j, k >= 0.
 * https://oeis.org/A108513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth smallest integer of the form 2^i*5^j*7^k
 *            (i,j,k>=0)
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate humble numbers with primes {2,5,7} using three indices. */
/* Initial terms: 1, 2, 4, 5, 7, 8, 10, 14, 16, ... */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* not a valid index; return 0 just in case */
        return 0;

    /* static storage for generated numbers and indices */
    static seq_t *buf = NULL;
    static size_t  size = 0;   /* number of elements stored (1-indexed) */
    static size_t  cap  = 0;   /* allocated capacity */

    static size_t idx2 = 0, idx5 = 0, idx7 = 0;  /* pointers for next multiples */

    /* ensure array has capacity for at least n elements */
    while (size < (size_t)n) {
        /* grow buffer if needed */
        if (size + 1 > cap) {
            size_t newcap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failed */
            buf = tmp;
            cap = newcap;
        }

        /* First element is always 1 */
        if (size == 0) {
            buf[0] = (seq_t)1;
            size = 1;
            continue;
        }

        /* Compute next candidate values */
        seq_t v2 = buf[idx2] * (seq_t)2;
        seq_t v5 = buf[idx5] * (seq_t)5;
        seq_t v7 = buf[idx7] * (seq_t)7;
        seq_t next = v2;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        /* Store the next humble number */
        buf[size++] = next;

        /* Increment indices that produced the next value */
        if (v2 == next) idx2++;
        if (v5 == next) idx5++;
        if (v7 == next) idx7++;
    }

    return buf[(size_t)n - 1];
}
