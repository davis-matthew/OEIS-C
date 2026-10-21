/*
 * A062865  When expressed in base 3 and then interpreted in base 6, is a
 *          multiple of the original number.
 * https://oeis.org/A062865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 177, u64 n <= 201,
 *            u128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A062865:
   a(n) is the nth non‑negative integer m such that when m is written
   in base 3 and reinterpreted as a base 6 number, the resulting value
   is an integer multiple of m.  The sequence starts 0,1,2,3,6,9,18… */

/* standard library for memory allocation */
#include <stdlib.h>

/* compute the nth term, with 1‑based index */
seq_t a(idx_t n)
{
    /* static storage for the terms that have already been found */
    static seq_t *terms   = NULL;
    static size_t  cap    = 0;      /* allocated capacity of terms */
    static size_t  count  = 0;      /* number of terms currently stored */
    static idx_t   m      = 0;      /* next candidate to test (starts at 0) */

    /* helper to grow the array when necessary */
    if (terms == NULL || cap == 0) {
        cap    = 64;
        terms  = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* special case: 0 is always a valid term */
        if (m == 0) {
            if (count >= cap) {
                cap   = 2 * cap;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[count++] = (seq_t)0;
            m = 1;                 /* next candidate is 1 */
            continue;
        }

        /* compute V = interpretation of m's base‑3 digits as base‑6 */
        seq_t V     = 0;
        seq_t pow6  = 1;
        idx_t mm    = m;          /* copy for digit extraction */

        while (mm > 0) {
            idx_t d = mm % 3;          /* current base‑3 digit (0‑2) */
            mm    /= 3;
            V      += (seq_t)d * pow6;
            pow6   *= 6;
        }

        /* check the divisibility condition */
        if (V % m == 0) {
            if (count >= cap) {
                cap   = 2 * cap;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[count++] = (seq_t)m;
        }

        ++m;                     /* try next candidate */
    }

    /* return the requested term (1‑based index) */
    return terms[n - 1];
}
