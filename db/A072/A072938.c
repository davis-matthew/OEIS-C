/*
 * A072938  Highly composite numbers that are half of the next highly
 *          composite number.
 * https://oeis.org/A072938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A072938 is the list of highly composite numbers m that are
 * exactly half of the next highly composite number.  There are no further
 * terms beyond 2520 (see W. Brefeld, 2013).  Therefore the sequence is
 * finite and the values are:
 * 1, 2, 6, 12, 60, 360, 2520
 * For n > 7 the value is defined as 0.
 */

#include <stddef.h>  // for size_t

seq_t a(idx_t n)
{
    /* Table of known terms.  Index 0 is unused to match the OEIS offset 1. */
    static const seq_t terms[] = {
        (seq_t)0,   /* placeholder for n = 0 */
        (seq_t)1,
        (seq_t)2,
        (seq_t)6,
        (seq_t)12,
        (seq_t)60,
        (seq_t)360,
        (seq_t)2520
    };
    const size_t count = sizeof(terms) / sizeof(terms[0]) - 1;  /* 7 terms */

    if (n >= 1 && n <= (idx_t)count)
        return terms[(size_t)n];
    else
        return (seq_t)0;
}
