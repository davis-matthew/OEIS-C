/*
 * A232345  Number of (n+1)X(1+1) 0..2 arrays with every element both >= and
 *          <= some horizontal, vertical, diagonal or antidiagonal neighbor.
 * https://oeis.org/A232345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A232345: Number of (n+1)×(1+1) 0..2 arrays with every element
   both ≥ and ≤ some horizontal, vertical, diagonal or antidiagonal
   neighbor.
   
   Empirical recurrence:
     a(n) = 7*a(n-1) - 2*a(n-2) + 30*a(n-3) + 44*a(n-4)
           + 8*a(n-5) + 16*a(n-6) - 32*a(n-7) - 64*a(n-8)
   Initial values:
     a(1)=21, a(2)=207, a(3)=1461, a(4)=10503,
     a(5)=77613, a(6)=575087, a(7)=4251717, a(8)=31416759
*/

#include <stddef.h>   /* for size_t if needed */

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* guard a(n) only defined for n>=1 */
    if (n < 1)
        return 0;

    /* initial eight terms (a(1)..a(8)) */
    static const seq_t init[8] = {
        21ULL,      /* a(1) */
        207ULL,     /* a(2) */
        1461ULL,    /* a(3) */
        10503ULL,   /* a(4) */
        77613ULL,   /* a(5) */
        575087ULL,  /* a(6) */
        4251717ULL, /* a(7) */
        31416759ULL /* a(8) */
    };

    /* if n is within the first eight terms, return directly */
    if (n <= 8)
        return init[n - 1];

    /* rolling window of the last eight known values:
       v[0] = a(k-8), v[1] = a(k-7), ..., v[7] = a(k-1)   */
    seq_t v[8];
    for (size_t i = 0; i < 8; ++i)
        v[i] = init[i];

    /* compute terms k = 9 .. n using the recurrence */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t next = 7 * v[7]          /* a(k-1) */
                    - 2 * v[6]         /* a(k-2) */
                    + 30 * v[5]        /* a(k-3) */
                    + 44 * v[4]        /* a(k-4) */
                    + 8 * v[3]         /* a(k-5) */
                    + 16 * v[2]        /* a(k-6) */
                    - 32 * v[1]        /* a(k-7) */
                    - 64 * v[0];       /* a(k-8) */

        /* shift window: drop the oldest and append the new value */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = v[7];
        v[7] = next;
    }

    /* v[7] now holds a(n) */
    return v[7];
}
