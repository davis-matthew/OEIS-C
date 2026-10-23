/*
 * A041601  Denominators of continued fraction convergents to sqrt(318).
 * https://oeis.org/A041601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041601: Denominators of continued fraction convergents to sqrt(318)
   Recurrence: a(n) = 214*a(n-4) – a(n-8)  for n > 7
   Initial values:
   a(0)=1, a(1)=1, a(2)=5, a(3)=6,
   a(4)=209, a(5)=215, a(6)=1069, a(7)=1284 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n < 8) {
        /* The first eight terms are known constants */
        static const seq_t init[8] = {
            1,   /* a(0) */
            1,   /* a(1) */
            5,   /* a(2) */
            6,   /* a(3) */
            209, /* a(4) */
            215, /* a(5) */
            1069,/* a(6) */
            1284 /* a(7) */
        };
        return init[(size_t)n];
    }

    /* Ring buffer holding the last 8 computed values.
       buf[i] holds a(i) for the current i modulo 8. */
    seq_t buf[8] = {
        1,   /* a(0) */
        1,   /* a(1) */
        5,   /* a(2) */
        6,   /* a(3) */
        209, /* a(4) */
        215, /* a(5) */
        1069,/* a(6) */
        1284 /* a(7) */
    };

    /* Compute a(n) iteratively from 8 up to n. */
    for (idx_t i = 8; i <= n; ++i) {
        /* a(i-4) and a(i-8) are stored at positions (i-4)%8 and (i-8)%8 */
        seq_t val = (seq_t)214 * buf[(i - 4) % 8] - buf[(i - 8) % 8];
        buf[i % 8] = val;
    }

    return buf[n % 8];
}
