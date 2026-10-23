/*
 * A037367  Numbers whose base-7 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑7 and base‑8 expansions have no digits in common.
   a(n) returns the n‑th such number (1‑based).  The function scans integers
   sequentially, keeping found terms in a static array that grows as needed.
   This implementation is valid in C11 and C++17, uses only the permitted
   operations on seq_t, and handles any calling order. */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* stored sequence values */
    static idx_t count_terms = 0; /* number of terms stored */
    static idx_t capacity   = 0;  /* capacity of the allocated array */
    static idx_t next_to_test = 1;/* next integer to examine */

    /* Ensure we have at least n terms computed */
    while (count_terms < n)
    {
        idx_t x = next_to_test++;
        unsigned int mask7 = 0, mask8 = 0;
        idx_t tmp = x;

        /* mask of digits base 7 */
        while (tmp)
        {
            mask7 |= 1u << (unsigned int)(tmp % 7);
            tmp  /= 7;
        }
        tmp = x;
        /* mask of digits base 8 */
        while (tmp)
        {
            mask8 |= 1u << (unsigned int)(tmp % 8);
            tmp   /= 8;
        }
        if ((mask7 & mask8) == 0)
        {
            /* enlarge buffer if necessary */
            if (count_terms >= capacity)
            {
                capacity = (capacity == 0) ? 16 : capacity << 1;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[count_terms++] = (seq_t)x;
        }
    }

    return terms[n - 1];
}
