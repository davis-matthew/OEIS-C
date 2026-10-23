/*
 * A039163  Numbers whose base-11 representation has the same number of 0's
 *          and 8's.
 * https://oeis.org/A039163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate natural numbers and keep those whose base‑11
   representation contains an equal number of 0's and 8's. */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* check a single number */
static int equal_zeros_eights(seq_t x)
{
    seq_t y = x;
    unsigned count0 = 0, count8 = 0;
    while (y > (seq_t)0) {
        unsigned d = (unsigned)(y % (seq_t)11);
        if (d == 0)          ++count0;
        else if (d == 8)     ++count8;
        y = y / (seq_t)11;
    }
    return count0 == count8;
}

/* cached data for memoised generation */
static seq_t *list   = 0;      /* array of found terms          */
static size_t  cap   = 0;      /* current capacity of the array */
static size_t  len   = 0;      /* number of terms stored       */
static seq_t  next   = 1;      /* next integer to test         */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* initialise storage if needed */
    if (!list) {
        cap = 16;
        #ifdef __cplusplus
        list = new seq_t[cap];
        #else
        list = (seq_t*)malloc(cap * sizeof(seq_t));
        #endif
    }

    /* generate terms until we have at least n of them */
    while (len < (size_t)n) {
        if (equal_zeros_eights(next)) {
            if (len == cap) {       /* grow array */
                size_t oldcap = cap;
                cap *= 2;
                seq_t *tmp;
                #ifdef __cplusplus
                tmp = new seq_t[cap];
                for (size_t i = 0; i < oldcap; ++i) tmp[i] = list[i];
                delete[] list;
                #else
                tmp = (seq_t*)realloc(list, cap * sizeof(seq_t));
                #endif
                list = tmp;
            }
            list[len++] = next;
        }
        ++next;
    }

    return list[n - 1];
}
