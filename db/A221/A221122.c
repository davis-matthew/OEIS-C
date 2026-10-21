/*
 * A221122  Number of n X 4 arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=21, a(2)=1950, a(3)=179300, a(4)=16436223, a(5)=1506094707,
 *            a(6)=138001217828, and for n>6
 *            a(n)=109*a(n-1)-1680*a(n-2)+8230*a(n-3)-13300*a(n-4)+2025*a(n-5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the terms iteratively, keeping only the
   last five values needed for the recurrence. No external dependencies
   are required beyond the standard library. */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly.  For n<7 we simply
       return the known seed values; this avoids any looping or
       dynamic allocation for small indices. */
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)1950;
    if (n == 3) return (seq_t)179300;
    if (n == 4) return (seq_t)16436223;
    if (n == 5) return (seq_t)1506094707;
    if (n == 6) return (seq_t)138001217828;

    /* Seed the last five values needed for the recurrence:
       b = a(n-5), c = a(n-4), d = a(n-3), e = a(n-2), f = a(n-1). */
    seq_t b = (seq_t)1950;          /* a(2) */
    seq_t c = (seq_t)179300;        /* a(3) */
    seq_t d = (seq_t)16436223;      /* a(4) */
    seq_t e = (seq_t)1506094707;    /* a(5) */
    seq_t f = (seq_t)138001217828;  /* a(6) */

    /* Iteratively compute a(7) through a(n). */
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = (seq_t)109 * f
                    - (seq_t)1680 * e
                    + (seq_t)8230 * d
                    - (seq_t)13300 * c
                    + (seq_t)2025 * b;

        /* Shift the window: move b->c, c->d, d->e, e->f, f->next. */
        b = c;
        c = d;
        d = e;
        e = f;
        f = next;
    }
    return f;
}
