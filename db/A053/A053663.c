/*
 * A053663  Numbers k such that 2*k+1 does not divide k!+1.
 * https://oeis.org/A053663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A053663
 *  a(n) = the nth integer k (k ≥ 1) such that (2k+1) does not divide (k! + 1)
 *  Offset: 1  (a(1) = 1)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq   = NULL;   /* sequence values */
    static idx_t   sz   = 0;      /* number of terms stored so far */
    static idx_t   cap  = 0;      /* allocated capacity */
    static idx_t   next = 1;      /* next k to test */

    if (n == 0)  /* offset not used; return 0 for safety */
        return 0;

    /* expand sequence until we have at least n terms */
    while (sz < n) {
        unsigned long long m = (unsigned long long)(2 * next + 1);
        unsigned long long prod = 1, i;

        /* compute k! mod m, with early exit when prod becomes 0 */
        for (i = 1; i <= next; ++i) {
            prod = (prod * i) % m;
            if (prod == 0)  /* cannot give -1 mod m */
                break;
        }

        /* check divisibility */
        if (prod != 0 && (prod + 1) % m == 0) {
            /* 2k+1 divides k!+1; skip this k */
        } else {
            /* 2k+1 does not divide k!+1; record k */
            if (sz == cap) {
                /* increase capacity */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* out of memory; return 0 as fallback */
                seq = tmp;
                cap = new_cap;
            }
            seq[sz++] = (seq_t)next;
        }
        ++next;
    }

    return seq[n - 1];
}
