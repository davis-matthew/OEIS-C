/*
 * A122470  Two digit numbers generated from the set {2,3,4}.
 * https://oeis.org/A122470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the n‑th integer >=23 that uses only digits 2,3,4 and has
 *            no two consecutive equal digits, listed in ascending order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1 for the value 23.  For n < 1 the
       result is undefined; return 0 to satisfy the compiler. */
    if (n < 1) return 0;

    /* Step 1: find the length L of the n‑th number. */
    idx_t L = 2;                 /* minimal length is 2 digits */
    idx_t cumulative = 0;        /* count of numbers with length < L */

    for ( ;; ) {
        /* Count of admissible numbers of length L:
           first digit: 3 choices; each subsequent digit: 2 choices
           (not equal to the previous one). */
        idx_t countLen = 3 * ((idx_t)1 << (L-1));
        if (cumulative + countLen >= n) break;
        cumulative += countLen;
        ++L;
    }

    /* Step 2: determine the position within numbers of length L. */
    idx_t groupSize = (idx_t)1 << (L-1);          /* numbers per first digit */
    idx_t pos          = n - cumulative - 1;      /* 0‑based index in this length */
    idx_t groupIdx     = pos / groupSize;         /* which first digit group */
    idx_t offsetInside = pos % groupSize;         /* index within the group */

    /* First digit of the number.  The groups correspond to 2,3,4. */
    int firstDigit = 2 + (int)groupIdx;
    seq_t result = (seq_t)firstDigit;

    /* Construct the rest of the digits.  Each step chooses one of two
       digits (not equal to the previous one).  The binary representation
       of offsetInside determines the sequence of choices. */
    int prev = firstDigit;
    idx_t remaining = L - 1;                      /* number of remaining digits */
    idx_t bits = offsetInside;

    for (idx_t i = remaining; i > 0; --i) {
        int bit = (bits >> (i-1)) & 1;
        int nextDigit;

        if (prev == 2) {
            nextDigit = (bit == 0) ? 3 : 4;
        } else if (prev == 3) {
            nextDigit = (bit == 0) ? 2 : 4;
        } else {          /* prev == 4 */
            nextDigit = (bit == 0) ? 2 : 3;
        }

        result = result * 10 + (seq_t)nextDigit;
        prev = nextDigit;
    }

    return result;
}
