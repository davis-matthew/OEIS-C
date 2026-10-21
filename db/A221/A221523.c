/*
 * A221523  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) + 21*a(n-2) + 58*a(n-3) + 79*a(n-4) + 
 *             32*a(n-5) + 23*a(n-6) + 4*a(n-7) + 8*a(n-8)   (n > 8)
 * Initial values: 
 *   a(1)=0, a(2)=42, a(3)=222, a(4)=1764, a(5)=12458,
 *   a(6)=90684, a(7)=654864, a(8)=4738970
 */

#include <stddef.h>   // for size_t

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n == 0) return (seq_t)0;

    /* Initial terms (1‑based index). 0th element unused. */
    static const seq_t init[9] = {
        (seq_t)0,        // dummy for 0
        (seq_t)0,        // a(1)
        (seq_t)42,       // a(2)
        (seq_t)222,      // a(3)
        (seq_t)1764,     // a(4)
        (seq_t)12458,    // a(5)
        (seq_t)90684,    // a(6)
        (seq_t)654864,   // a(7)
        (seq_t)4738970   // a(8)
    };

    if (n <= 8) return init[n];

    /* Sliding window of the last 8 computed values.
       prev[0] = a(i-1), prev[1] = a(i-2), …, prev[7] = a(i-8). */
    seq_t prev[8];
    prev[0] = init[8];
    prev[1] = init[7];
    prev[2] = init[6];
    prev[3] = init[5];
    prev[4] = init[4];
    prev[5] = init[3];
    prev[6] = init[2];
    prev[7] = init[1];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = (seq_t)0;
        next += (seq_t)3 *  prev[0];
        next += (seq_t)21 * prev[1];
        next += (seq_t)58 * prev[2];
        next += (seq_t)79 * prev[3];
        next += (seq_t)32 * prev[4];
        next += (seq_t)23 * prev[5];
        next += (seq_t)4  * prev[6];
        next += (seq_t)8  * prev[7];

        /* Shift window: drop the oldest (prev[7]) and insert new at prev[0] */
        for (int j = 7; j > 0; --j) prev[j] = prev[j-1];
        prev[0] = next;
    }
    return prev[0];
}
