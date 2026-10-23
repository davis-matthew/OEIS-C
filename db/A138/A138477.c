/*
 * A138477  A084175 interleaved with 2*A084175.
 * https://oeis.org/A138477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138477 – OEIS sequence
 *
 *  Recurrence form (derived from the OEIS recurrence)
 *    a(0) = 0,   a(1) = 0,   a(2) = 1,   a(3) = 2,   a(4) = 3
 *    a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - 4*a(n-5)   for n ≥ 5
 *
 *  The function below implements this recurrence iteratively and returns
 *  a(n) for any n ≥ 0.  No dynamic allocation or memoisation is
 *  required, so the code works for both plain unsigned integer types
 *  and an arbitrary‑precision seq_t (C++ class) without using `new` or
 *  other language extensions.  It runs in O(n) time with a tiny,
 *  fixed amount of memory.
 */

#include <stddef.h>   /* for size_t, used only by idx_t if needed */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* initial values a(0)…a(4) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;

    /* ring buffer holding the last 5 computed terms:
     *   prev[0] = a(n-5), prev[1] = a(n-4), …, prev[4] = a(n-1)
     */
    seq_t prev[5] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3 };  /* a0…a4 */
    seq_t cur = 0;

    /* iterate from i = 5 up to n calculating a(i) */
    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-2) + 2*a(i-3) + 2*a(i-4) - 4*a(i-5) */
        cur = 2 * prev[4]       /* a(i-1)            */
              - prev[3]         /* a(i-2)            */
              + 2 * prev[2]     /* a(i-3)            */
              + 2 * prev[1]     /* a(i-4)            */
              - 4 * prev[0];    /* a(i-5)            */

        /* shift the ring buffer for the next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = cur;
    }

    return cur;
}
