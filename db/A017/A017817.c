/*
 * A017817  a(n) = a(n-3) + a(n-4), with a(0)=1, a(1)=a(2)=0, a(3)=1.
 * https://oeis.org/A017817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4); a(0)=1, a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 33, u16 n <= 61, u32 n <= 117, u64 n <= 228,
 *            u128 n <= 451, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following function returns the nth term of A017817.
   It uses only the operations allowed for seq_t in the build and
   works for any integer-width or arbitrary‑precision seq_t.  */
seq_t a(idx_t n)
{
    /* Handle small indices directly.  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* Allocate an array large enough for indices 0..n.  */
    size_t len = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (buf == NULL) {
        /* In case of allocation failure, return 0 (sequence value
           is not defined).  The build is expected to provide a
           suitable allocator for seq_t.  */
        return (seq_t)0;
    }

    /* Initialize the first four values.  */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)0;
    buf[2] = (seq_t)0;
    buf[3] = (seq_t)1;

    /* Compute the recurrence for i = 4 .. n.  */
    for (idx_t i = 4; i <= n; ++i)
        buf[i] = buf[i - 3] + buf[i - 4];

    seq_t result = buf[n];
    free(buf);
    return result;
}
