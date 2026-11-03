/*
 * A054484  Numbers not divisible by any of their digits when written in base
 *          7.
 * https://oeis.org/A054484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers not divisible by any of their digits when written in base 7 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool good_candidate(uint64_t n)
{
    uint64_t m = n;
    while (m > 0) {
        int d = m % 7;   /* base-7 digit */
        if (d == 1)          /* digit 1 always divides n */
            return false;
        if (d > 1 && (n % d) == 0)
            return false;
        m /= 7;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* OEIS offset: 1, so a[0] corresponds to n=1 */
    static seq_t *terms = NULL;
    static idx_t len   = 0;   /* number of terms currently stored */
    static idx_t cap   = 0;   /* allocated capacity of the array */
    static uint64_t next = 1; /* next natural number to test */

    if (n == 0)        /* out of range, but undefined for OEIS */
        return (seq_t)0;

    while (len < n) {
        while (!good_candidate(next))
            ++next;

        /* store next as a new term */
        seq_t val = (seq_t)next;

        /* grow storage if necessary */
        if (len >= cap) {
            if (cap == 0) cap = 16;
            else          cap <<= 1;
            terms = mem_resize(seq_t, terms, cap);
        }
        terms[len++] = val;
        ++next;
    }
    return terms[n-1];
}
