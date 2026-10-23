/*
 * A116704  Number of permutations of length n which avoid the patterns 2341,
 *          4312.
 * https://oeis.org/A116704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 22*a(n-3) - 13*a(n-4) + 4*a(n-5)
 *            for n > 6 Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6,
 *            a(4)=22, a(5)=86, a(6)=338
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 0..for the largest computed index so far   */
    static seq_t *buf   = NULL;
    static size_t last  = (size_t)-1;      /* no entries computed yet    */
    static size_t cap   = 0;               /* capacity of allocated buf  */
    /* initial five values needed for recurrence + first 7 terms */
    static const seq_t INIT[7] = {1, 1, 2, 6, 22, 86, 338};

    /* first call: allocate and initialise the first 7 terms */
    if (cap == 0) {
        cap = 16;                          /* start with some room    */
        buf = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i)
            buf[i] = INIT[i];
        last = 6;
    }

    /* if we are asked for a value we already know */
    if ((size_t)n <= last)
        return buf[(size_t)n];

    /* ensure we have enough space */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        buf = tmp;
        cap = new_cap;
    }

    /* compute up to n using the recurrence */
    for (size_t k = last + 1; k <= (size_t)n; ++k) {
        if (k <= 6) {
            /* base values already stored */
            continue;
        }
        /* all operations are performed in seq_t type only */
        seq_t val = ((seq_t)7)  * buf[k-1]
                  + ((seq_t)22) * buf[k-3]
                  + ((seq_t)4)  * buf[k-5]
                  - ((seq_t)17) * buf[k-2]
                  - ((seq_t)13) * buf[k-4];
        buf[k] = val;
    }

    last = (size_t)n;
    return buf[(size_t)n];
}
