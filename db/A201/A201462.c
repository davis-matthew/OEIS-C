/*
 * A201462  Numbers that are not coprime to their 9's complement.
 * https://oeis.org/A201462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 4217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are not coprime to their 9's complement
   a(n) is the nth positive integer m such that
   gcd(m, 10^d - 1 - m) > 1, where d is the number of decimal digits of m. */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    static seq_t *list   = NULL;   /* cached terms */
    static size_t  len   = 0;      /* number of cached terms */
    static size_t  cap   = 0;      /* allocated capacity */
    static idx_t   next  = 1;      /* next candidate to test */

    if (n < 1) return 0;           /* undefined for n<1 */

    /* Generate terms until we have at least n of them */
    while (len < n) {
        /* Ensure enough space for one more term */
        if (len + 1 > cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
#ifdef __cplusplus
            void *tmp = realloc(list, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out of memory */
            list = static_cast<seq_t*>(tmp);
#else
            list = (seq_t*)realloc(list, new_cap * sizeof(seq_t));
            if (!list) abort();          /* out of memory */
#endif
            cap = new_cap;
        }

        /* Test the next candidate integer */
        idx_t  cand    = next++;
        seq_t  cand_s  = (seq_t)cand;
        int    d       = digit_len((idx_t)cand);          /* number of decimal digits */
        seq_t  pow10d  = pow10(d);                       /* 10^d as seq_t */
        seq_t  comp    = pow10d - 1 - cand_s;            /* 9's complement of cand */
        seq_t  g       = gcd(cand_s, comp);              /* gcd(cand, complement) */

        if (g > (seq_t)1) {
            /* Store the term */
            list[len++] = cand_s;
        }
    }

    return list[n - 1];
}
