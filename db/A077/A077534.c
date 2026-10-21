/*
 * A077534  Multiples of 4 using only prime digits (2, 3, 5 and 7).
 * https://oeis.org/A077534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   This function generates the nth term of OEIS A077534, the
 *            sequence of integers whose decimal representation uses only the
 *            prime digits {2,3,5,7}, ends with a 2, the second‑to‑last digit
 *            is odd (3,5, or 7), and the remaining higher digits may be any
 *            of the four prime digits. The terms are returned...
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine the length L of the nth term:
       the cumulative count up to length (L-1) is 4^{L-2} - 1.
       We need the smallest L such that 4^{L-1} >= n+1. */
    idx_t target = n + 1;          /* we compare against n+1 */
    idx_t L = 2;                   /* start from length 2 */
    idx_t pow4 = 4;                /* 4^(L-1) */
    while (pow4 < target) {
        pow4 *= 4;                 /* increase exponent by 1 */
        ++L;
    }

    /* Number of terms of shorter lengths (before this length). */
    idx_t prevTotal = (pow4 / 4) - 1;   /* 4^{L-2} - 1 */

    /* Index within current length (1‑based). */
    idx_t idxWithin = n - prevTotal;    /* 1 .. 3*4^{L-2} */

    /* Prepare constants for digit choices. */
    const seq_t oddDigits[3]   = {3, 5, 7};   /* must be odd: tens place */
    const seq_t primeDigits[4] = {2, 3, 5, 7};/* any prime digit for other places */

    seq_t number = 2;  /* units digit is always 2 */
    idx_t i = idxWithin - 1;      /* 0‑based index within this length */

    /* Tens digit (odd). */
    seq_t tens = oddDigits[i % 3];
    number += tens * 10;
    i /= 3;

    /* Remaining higher positions (L-2 of them). */
    seq_t pow10 = 100;  /* 10^2 for the first higher digit (hundreds) */
    for (idx_t pos = 0; pos < L - 2; ++pos) {
        seq_t digit = primeDigits[i % 4];
        number += digit * pow10;
        i /= 4;
        pow10 *= 10;
    }

    return number;
}
