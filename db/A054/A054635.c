/*
 * A054635  Champernowne sequence: write n in base 3 and juxtapose.
 * https://oeis.org/A054635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf, changed
 * Formula:   a(n) is the n-th digit (0‑indexed) in the infinite string
 *            obtained by writing each non‑negative integer in base‑3 and
 *            concatenating them: 0,1,2,10,11,12,20,21,22,100,…
 * Range:     u8 n <= 5914, u16 n <= 5914, u32 n <= 5914, u64 n <= 5914,
 *            u128 n <= 5914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the block of base‑3 numbers whose total
       digits exceed n.  Each block contains all numbers
       with the same length L. */
    unsigned __int128 cumulative = 0;   /* total digits so far */
    unsigned __int128 pow3 = 1;         /* 3^(L-1) for the current block */
    idx_t L = 1;                        /* current block length */

    while (1) {
        unsigned __int128 count;
        if (L == 1)          /* numbers 0,1,2 */
            count = 3;
        else                 /* 2 * 3^(L-1) numbers of length L */
            count = 2 * pow3;

        unsigned __int128 blockDigits = count * L;
        if (cumulative + blockDigits > (unsigned __int128)n)
            break;           /* n lies inside this block */

        cumulative += blockDigits;
        pow3 *= 3;           /* prepare 3^L for next block */
        ++L;
    }

    /* Position of n inside the chosen block */
    unsigned __int128 offset     = (unsigned __int128)n - cumulative;
    idx_t           digitIndex  = (idx_t)(offset % L);      /* 0‑based leftmost */
    unsigned __int128 numberIndex = offset / L;

    unsigned __int128 startNumber;
    if (L == 1)
        startNumber = 0;
    else
        startNumber = pow3;   /* numbers of length L start at 3^(L-1) */

    unsigned __int128 number = startNumber + numberIndex;

    /* Extract the desired digit: divide by 3^(L-1-digitIndex) */
    unsigned __int128 divisor = 1;
    idx_t exp = L - 1 - digitIndex;
    for (idx_t i = 0; i < exp; ++i)
        divisor *= 3;

    unsigned __int128 digit = (number / divisor) % 3;
    return (seq_t)digit;
}
