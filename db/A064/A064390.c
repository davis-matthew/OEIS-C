/*
 * A064390  Numbers of the form 2^k*(2^n+1) or 2^k*(4^n-2^n+1).
 * https://oeis.org/A064390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064390: numbers of the form 2^k*(2^n+1) or 2^k*(4^n-2^n+1).
   Implementation: initialise all base values B1 = 2^n+1 and B2 = 4^n-2^n+1
   for n = 0..31, then repeatedly emit the smallest current value and
   double the corresponding sequence to produce the infinite merge. */

/* The function may be compiled either as C11 or C++17; seq_t may be an
   unsigned integer type (uint8_t ... uint64_t, unsigned __int128) or an
   arbitrary‑precision integer class with the usual operators. */
#include <stddef.h>
#include <stdlib.h>

static const unsigned MAX_M = 31;      /* enough for the first 10 000 terms */

seq_t a(idx_t n)
{
    /* number of different base sequences: two per m (≤MAX_M) */
    const size_t numBases = 2 * (MAX_M + 1);
    seq_t cur[numBases];     /* current value of each base sequence */

    /* initialise each sequence with its base value  */
    for (unsigned m = 0; m <= MAX_M; ++m) {
        seq_t pow2_m = ((seq_t)1 << m);          /* 2^m   */
        cur[2 * m] = pow2_m + 1;                 /* 2^m + 1 */
        seq_t pow4_m = pow2_m * pow2_m;          /* 4^m   */
        cur[2 * m + 1] = pow4_m - pow2_m + 1;   /* 4^m - 2^m + 1 */
    }

    idx_t count = 0;
    seq_t last = 0;
    int first = 1;     /* flag for the very first output */

    /* Merge the sequences in ascending order */
    while (1) {
        /* find the minimal current value */
        size_t idx_min = 0;
        for (size_t i = 1; i < numBases; ++i)
            if (cur[i] < cur[idx_min])
                idx_min = i;

        seq_t curmin = cur[idx_min];

        /* output unique values in sorted order */
        if (first || curmin != last) {
            ++count;
            if (count == n)
                return curmin;
            last = curmin;
            first = 0;
        }

        /* advance this sequence: multiply by 2 */
        cur[idx_min] *= 2;   /* can also use <<=1 for all types */
    }
}
