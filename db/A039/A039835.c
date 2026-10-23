/*
 * A039835  Indices of triangular numbers which are also heptagonal.
 * https://oeis.org/A039835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039835: indices of triangular numbers which are also heptagonal.
 * The sequence satisfies the linear recurrence
 *
 *   a(n) = a(n‑1) + 322·a(n‑2) – 322·a(n‑3) – a(n‑4) + a(n‑5)   for n ≥ 6
 *
 * with initial values
 *
 *   a(1) = 1,  a(2) = 10,  a(3) = 493,  a(4) = 3382,  a(5) = 158905.
 *
 * The function below evaluates a(n) directly, using only the recurrence
 * and no caching, so it works correctly for arbitrary-precision seq_t
 * and for 8/16/32/64-bit integer builds.  It is O(n) in time and O(1)
 * in space.
 */

#include <stdint.h>  /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour is irrelevant. */
    if (n < 1)
        return (seq_t)0;

    /* Handle the first five terms directly. */
    switch (n) {
    case 1: return (seq_t)1;
    case 2: return (seq_t)10;
    case 3: return (seq_t)493;
    case 4: return (seq_t)3382;
    case 5: return (seq_t)158905;
    }

    /* For n ≥ 6, iterate using the recurrence. */
    seq_t a1 = (seq_t)1,      /* a(n‑5)   */
           a2 = (seq_t)10,     /* a(n‑4)   */
           a3 = (seq_t)493,    /* a(n‑3)   */
           a4 = (seq_t)3382,   /* a(n‑2)   */
           a5 = (seq_t)158905; /* a(n‑1)   */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        next = a5 + (seq_t)322 * a4 - (seq_t)322 * a3 - a2 + a1;
        a1 = a2;   /* shift the window one step forward */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
