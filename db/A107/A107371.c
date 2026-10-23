/*
 * A107371  Numbers k such that 101*k - 103 is prime.
 * https://oeis.org/A107371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 4509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7357, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 101*k - 103 is prime.  a(1)=6, a(2)=10, … */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t) */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static cache of k-values */
    static seq_t *ks    = NULL;  /* array of found k's */
    static idx_t  cap   = 0;     /* current capacity of ks */
    static idx_t  size  = 0;     /* number of elements stored */
    static uint64_t next = 1;    /* next k to test */

    /* grow the array as needed */
    const idx_t grow = (cap == 0) ? 4 : cap * 2;

    /* generate terms until we have at least n of them */
    while (size < n) {
        uint64_t val = 101ULL * next - 103ULL;   /* candidate prime */
        if (is_prime(val)) {
            if (size == cap) {
                cap   = grow;
                ks    = (seq_t *)realloc(ks, cap * sizeof(seq_t));
            }
            ks[size++] = (seq_t)next;           /* store the k-value */
        }
        ++next;
    }

    /* offset is 1 (a(1) is first term) */
    return ks[n - 1];
}
