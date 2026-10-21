/*
 * A201245  Number of ways to place 4 non-attacking ferses on an n X n board.
 * https://oeis.org/A201245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 30n^6 + 48n^5 + 299n^4 - 912n^3 - 462n^2 + 4368n -
 *            4200)/24 for n>=3, a(1)=a(2)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A201245_H
#define SEQ_A201245_H
#include <stdint.h>
/* External integer types supplied by the build. */
extern seq_t a(idx_t n);
#endif

seq_t a(idx_t n)
{
    /* Handle the early zero terms. */
    if (n < 3)
        return (seq_t)0;

    /* All intermediate calculations are done in an unsigned 128‑bit
       integer to avoid overflow for n up to at least 10^4. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n2 * n2;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n3 * n3;   /* n^6 = (n^3)^2 */
    unsigned __int128 n8 = n4 * n4;

    unsigned __int128 numerator =
        n8
        - (unsigned __int128)30 * n6
        + (unsigned __int128)48 * n5
        + (unsigned __int128)299 * n4
        - (unsigned __int128)912 * n3
        - (unsigned __int128)462 * n2
        + (unsigned __int128)4368 * N
        - (unsigned __int128)4200;

    /* The division by 24 yields an integer for all n >= 3. */
    seq_t result = (seq_t)(numerator / (unsigned __int128)24);
    return result;
}
