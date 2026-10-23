/*
 * A143712  Numbers with at least two digits in which all digits except the
 *          rightmost are even and the rightmost is odd.
 * https://oeis.org/A143712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) enumerates the n‑th integer ≥10 whose all digits except the
 *            * rightmost are even and whose rightmost digit is odd. The
 *            algorithm * determines the number of decimal digits, then
 *            reconstructs the number * in lexicographic order. It runs in
 *            O(log₅(n)) time
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n<1, but harmless */

    /* Find the length L for which n lies among L‑digit numbers.
     * Count of L‑digit numbers: 20 * 5^(L-2). */
    unsigned __int128 cum = 0;        /* cumulative count of numbers with <= L-1 digits */
    unsigned __int128 pow5 = 1;       /* 5^(L-2)  */
    int L = 2;
    for (;;) {
        unsigned __int128 f = 20 * pow5;          /* numbers with length L */
        if (cum + f >= (unsigned __int128)n) break;
        cum += f;
        pow5 *= 5;
        ++L;
    }

    /* Index of the desired number in the block of L‑digit numbers (0‑based). */
    unsigned __int128 rem = (unsigned __int128)n - cum - 1;

    /* Weight of the most significant digit (number of combinations after it). */
    unsigned __int128 weight0 = pow5 * 5;          /* 5^(L-1) */
    int first_idx = (int)(rem / weight0);
    rem %= weight0;

    /* Convert the remaining part to base‑5, length L‑1. */
    int base[64] = {0};
    for (int i = 0; i < L - 1; ++i) {
        base[i] = (int)(rem % 5);
        rem /= 5;
    }

    /* Digit tables. */
    static const int first_map[4] = {2, 4, 6, 8};
    static const int mid_map[5]   = {0, 2, 4, 6, 8};
    static const int last_map[5]  = {1, 3, 5, 7, 9};

    /* Build the number. */
    seq_t result = first_map[first_idx];
    for (int j = L - 2; j >= 1; --j) {
        result = result * 10 + mid_map[base[j]];
    }
    result = result * 10 + last_map[base[0]];

    return result;
}
