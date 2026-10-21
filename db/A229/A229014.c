/*
 * A229014  Number of arrays of median of three adjacent elements of some
 *          length 6 0..n array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>5 Initial values: a(1)=2, a(2)=31, a(3)=130, a(4)=359,
 *            a(5)=794 (The same recurrence follows from the polynomial a(n) =
 *            (2/3)n^4 + (10/3)n^3 - (5/3)n^2 + (2/3)n - 1.)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)130;
    if (n == 4) return (seq_t)359;
    if (n == 5) return (seq_t)794;

    /* Variables to hold the last five values of the sequence.
       They are stored as seq_t but intermediate calculations use
       signed 64‑bit integers to allow subtractions.  For all
       n <= 10 000 the values comfortably fit in 64 bits. */
    seq_t a1 = (seq_t)2;      /* a(n-5) */
    seq_t a2 = (seq_t)31;     /* a(n-4) */
    seq_t a3 = (seq_t)130;    /* a(n-3) */
    seq_t a4 = (seq_t)359;    /* a(n-2) */
    seq_t a5 = (seq_t)794;    /* a(n-1) */
    seq_t ai  = a5;           /* will hold a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        long long t = 5LL * (long long)a5
                    -10LL * (long long)a4
                    +10LL * (long long)a3
                    -5LL  * (long long)a2
                    + (long long)a1;
        ai = (seq_t)t;

        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }

    return ai;
}
