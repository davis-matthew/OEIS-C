/*
 * A135663  a(n) = floor(sqrt(n) - n^(1/4)).
 * https://oeis.org/A135663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n) - n^(1/4))
 * Range:     u8 n <= 10, u16 n <= 956, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A135663 using integer arithmetic only. */
/* Uses the helper floor_sqrt which returns floor(sqrt(x)) for any seq_t. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                  /* convert index to seq_t */
    seq_t s  = floor_sqrt(sn);            /* floor(sqrt(n))       */
    seq_t r  = floor_sqrt(s);             /* floor(n^(1/4))       */
    seq_t k  = s - r;                     /* k = s - r            */

    /* left  = (1 + 2k)^2 * n  */
/* right = (n + k^2)^2   */
    seq_t two_k = 2 * k;
    seq_t t      = 1 + two_k;            /* 1 + 2k               */
    seq_t t2     = t * t;                /* (1 + 2k)^2           */
    seq_t left   = t2 * sn;              /* left  part            */

    seq_t k2     = k * k;                /* k^2                  */
    seq_t sum    = sn + k2;              /* n + k^2              */
    seq_t right  = sum * sum;            /* right part           */

    /* floor(sqrt(n) - n^(1/4)) is k - 1 when left > right, otherwise k. */
    if (left > right) {
        return k - 1;
    } else {
        return k;
    }
}
