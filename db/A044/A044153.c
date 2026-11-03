/*
 * A044153  Numbers n such that string 2,1 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the nth positive integer n (n ≥ 1)
   such that the base‑7 representation of n contains the digit
   sequence 2,1 but the representation of n−1 does not.
   The implementation performs a simple incremental search over
   successive integers. */

#include <stdbool.h>

/* Test whether x in base 7 contains the sequence "21". */
static bool contains21(seq_t x)
{
    seq_t prev = (seq_t)0;
    seq_t d;
    const seq_t seven = (seq_t)7;

    while (x > (seq_t)0) {
        d = x % seven;
        if (d == (seq_t)2 && prev == (seq_t)1)
            return true;          /* digits ...2,1 found */
        prev = d;
        x = x / seven;
    }
    return false;
}

/* OEIS sequence A044153. */
seq_t a(idx_t n)
{
    if (n == 0)           /* sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    seq_t val = (seq_t)1;      /* start checking from 1 */
    idx_t found = 0;
    bool prev_has = false;     /* contains21(val-1) for the first loop */

    for ( ; ; ++val) {
        bool cur_has = contains21(val);

        if (cur_has && !prev_has) {      /* val meets the criterion */
            ++found;
            if (found == n)
                return val;
        }

        prev_has = cur_has;
    }
}
