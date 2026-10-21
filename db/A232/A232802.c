/*
 * A232802  Number of solution pairs (x,y) for x <= 11 such that x! + n = y^2
 *          (Brocard-Ramanujan Diophantine equation) is soluble over the
 *          integers.
 * https://oeis.org/A232802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of solution pairs (x,y) such that 1 ≤ x ≤ 11 and x! + n = y².  */
/* The function returns a( n ) for the OEIS sequence A232802.  */

/* The library provides a sequence–type independent integer square root.  */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Factorials 0! … 11! – stored as seq_t to avoid casting during the loop. */
static const seq_t fact_arr[12] = {
    (seq_t)1ULL,               /* 0! */
    (seq_t)1ULL,               /* 1! */
    (seq_t)2ULL,               /* 2! */
    (seq_t)6ULL,               /* 3! */
    (seq_t)24ULL,              /* 4! */
    (seq_t)120ULL,             /* 5! */
    (seq_t)720ULL,             /* 6! */
    (seq_t)5040ULL,            /* 7! */
    (seq_t)40320ULL,           /* 8! */
    (seq_t)362880ULL,          /* 9! */
    (seq_t)3628800ULL,         /* 10! */
    (seq_t)39916800ULL         /* 11! */
};

/* Return a(n) for n ≥ 1.  /* By definition a(n) = 0 for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t count = 0;
    idx_t i;

    for (i = 1; i <= 11; ++i) {
        seq_t val = (seq_t)n + fact_arr[i];
        seq_t sq  = floor_sqrt(val);
        if (sq * sq == val)
            ++count;
    }
    return count;
}
