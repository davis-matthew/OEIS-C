/*
 * A072724  Integers which are exactly the concatenation of the first m even
 *          numbers (A019520) divided by their sum (A002378 = m^2+m).
 * https://oeis.org/A072724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   mem, num_digits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072724
   Formula: a(n) = C(m) / (m*(m+1))
   where C(m) is the decimal concatenation of the first m even numbers
   (2,4,6,…,2m), and m is the n‑th value for which the division is
   an integer.  The function scans successive values of m, building
   C(m) incrementally, and stores all quotients that are integral. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits_u64.h"
#include "../utils/mem.h"

/* append the next even number (2*m) to the current concatenation C.
   C is updated in place.  pow10 is 10^digits(2*m) as a seq_t. */
static void append_next_even(seq_t *C, uint64_t m)
{
    uint64_t next = 2 * m;                   /* next even number */
    int digits = num_digits_u64(next);      /* digits of next */
    seq_t pow10 = (seq_t)1;                 /* 10^digits */
    for (int i = 0; i < digits; ++i)
        pow10 *= (seq_t)10;
    *C = (*C) * pow10 + (seq_t)next;        /* C ← C*10^digits + next */
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len   = 0;          /* number of stored terms   */
    static size_t cache_alloc = 0;          /* allocated cache size     */
    static uint64_t last_m   = 0;           /* last m processed         */
    static seq_t C = (seq_t)0;              /* current concatenation    */

    /* ensure enough space for future entries */
    if (cache_len + 1 > cache_alloc) {
        size_t new_alloc = cache_alloc ? cache_alloc * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, new_alloc);
        if (!tmp) {                         /* allocation failed */
            mem_free(cache);
            cache = NULL;
            cache_len = 0;
            cache_alloc = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_m;                           /* advance to next m       */
        append_next_even(&C, last_m);        /* build C(m)              */
        seq_t d = (seq_t)last_m * (last_m + 1); /* denominator m(m+1)   */
        if ((C % d) == 0) {                 /* check divisibility      */
            cache[cache_len++] = C / d;     /* store the quotient      */
        }
    }

    return cache[(size_t)n - 1];
}
