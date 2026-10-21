/*
 * A053803  Numbers where the difference of consecutive cubes is "close" to
 *          another cube: let A = x^3 - (x-1)^3, sequence is the x's where A -
 *          int(A^(1/3))^3 < int(x^(1/2))^3.
 * https://oeis.org/A053803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   sqrt_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A053803: a(n) = the n‑th integer x ≥ 1 for which  
   x³ – (x–1)³ – ⌊ (x³ – (x–1)³)¹⁄³ ⌋³  <  ⌊ √x ⌋³.  
   The function computes the sequence by an incremental search
   (starting at x = 1) and memoises the results.  No hard‑coded
   table is used; the routine works for any seq_t implementation
   (fixed‑width or arbitrary precision).  The inequality is checked
   with exact integer arithmetic.  On building the result we
   evaluate the integer cube root and integer square root by
   binary search on the seq_t type, avoiding any overflow.
   OEIS offset = 1. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sqrt_root_floor.h"

/* Forward declaration of the index type – supplied by the build. */
typedef uint64_t idx_t;

/* --------------------------- helpers ---------------------------------- */

/* Compute floor of value^(1/3) using binary search on seq_t. */
static seq_t cube_root_floor(seq_t value)
{
    seq_t low = 0;
    seq_t one = 1;
    seq_t high = 1;
    /* Find an upper bound: double until high³ > value. */
    while (high * high * high <= value) {
        high = high + high;   /* high *= 2; */
    }
    /* Binary search for the floor cube root. */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= value) {
            low = mid + one;
        } else {
            high = mid - one;
        }
    }
    return high;
}

/* Check whether a given x satisfies the defining inequality. */
static bool qualifies(idx_t x)
{
    /* A = x³ – (x–1)³ = 3x² – 3x + 1.  Compute with seq_t. */
    seq_t X  = (seq_t)x;
    seq_t A  = (seq_t)3 * X * X - (seq_t)3 * X + (seq_t)1;

    seq_t c   = cube_root_floor(A);          /* floor cube root of A   */
    seq_t diff = A - c * c * c;              /* A – floor(A^(1/3))³   */

    seq_t s   = sqrt_root_floor((seq_t)x);    /* floor sqrt(x)         */
    seq_t rhs = s * s * s;                   /* (floor √x)³           */

    return diff < rhs;
}

/* ------------------------- main routine ------------------------------- */

static seq_t *vals   = NULL;     /* memoised terms */
static size_t val_cnt = 0;       /* number of stored terms */
static size_t val_cap = 0;       /* allocated array capacity */
static idx_t  next_x  = 1;       /* next integer to test */

/* Grow the memoisation array if needed. */
static void grow_val_array(void)
{
    size_t new_cap = val_cap ? val_cap << 1 : 128;
    while (new_cap < val_cnt + 1) {
        new_cap <<= 1;
    }
    vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
    val_cap = new_cap;
}

/* Required external function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety guard – not used by the tests */

    /* Extend the cache until it contains the nth term. */
    while (val_cnt < n) {
        if (qualifies(next_x)) {
            if (val_cnt == val_cap)
                grow_val_array();
            vals[val_cnt++] = (seq_t)next_x;
        }
        ++next_x;
    }
    return vals[n - 1];
}
