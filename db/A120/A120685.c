/*
 * A120685  Integers m such that the sequence defined by f(0)=m and
 *          f(n+1)=1+gpf(f(n)), with gpf(n) being the greatest prime factor of
 *          n (A006530), ends up in the repetitive cycle 4 -> 3 -> 4 -> ...
 * https://oeis.org/A120685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A120685:  integers m for which, when iterating
 *  f(0)=m, f(k+1)=1+gpf(f(k)) (gpf = greatest prime factor),
 *  the first time the value 4 occurs is after an even number of steps.
 *
 *  The function a(idx_t n) returns the n‑th element (a(0)=2, a(1)=4,
 *  a(2)=5, ...).  It builds the sequence lazily and caches the
 *  computed values.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gpf.h"    /* provides uint64_t gpf(uint64_t) */

static bool a120685_qualifies(uint64_t m)
{
    uint64_t val = m;
    uint64_t step = 0;
    for (;;) {
        step++;
        val = 1 + gpf(val);     /* next value of f */
        if (val == 4ULL)
            return (step & 1ULL) == 0;   /* even step -> accept */
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;    /* cached sequence values          */
    static idx_t  size = 0;      /* number of cached terms          */
    static idx_t  capacity = 0;  /* allocated length in seq[]       */
    static uint64_t next_m = 1;  /* next candidate integer to test  */

    /* Fast return if already cached */
    if (n < size)
        return seq[n];

    /* Lazy build of sequence up to index n */
    if (capacity == 0) {
        capacity = 128;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    while (size <= n) {
        /* find the next integer that satisfies the criterion */
        do {
            next_m++;
        } while (!a120685_qualifies(next_m));

        /* grow allocation if necessary */
        if (size == capacity) {
            capacity *= 2;
            seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
        }

        seq[size++] = (seq_t)next_m;
    }

    return seq[n];
}
