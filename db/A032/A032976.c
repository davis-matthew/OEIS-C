/*
 * A032976  Numbers with the property that all pairs of consecutive base-5
 *          digits differ by 0 or 1.
 * https://oeis.org/A032976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 2376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A032976:
   numbers whose base‑5 digits differ by 0 or 1 between consecutive digits.
   The implementation constructs the term by counting admissible digit
   strings combinatorially, avoiding explicit enumeration. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Maximum length of base‑5 representation needed for n up to 10⁶
     * (much more than required for n ≤ 10⁴). */
    enum { MAXLEN = 50 };

    /* dpLen[rem][prev] = number of admissible strings of length rem+1
       that start with digit prev.  rem is the number of positions
       remaining after the current digit (including the current one).   */
    static idx_t dpLen[MAXLEN][5];
    static idx_t lenCount[MAXLEN];   /* number of admissible numbers of a given length */
    static bool init = false;

    if (!init) {
        /* Base case: one string of length 1 for each starting digit */
        for (int d = 0; d < 5; ++d)
            dpLen[0][d] = 1;

        /* Fill table for longer lengths */
        for (int rem = 1; rem < MAXLEN; ++rem) {
            for (int prev = 0; prev < 5; ++prev) {
                idx_t sum = 0;
                for (int nxt = prev - 1; nxt <= prev + 1; ++nxt) {
                    if (nxt >= 0 && nxt < 5)
                        sum += dpLen[rem - 1][nxt];
                }
                dpLen[rem][prev] = sum;
            }
        }

        /* Count admissible numbers of each length (first digit 1-4) */
        for (int len = 1; len < MAXLEN; ++len) {
            idx_t sum = 0;
            for (int d = 1; d < 5; ++d)
                sum += dpLen[len - 1][d];
            lenCount[len] = sum;
        }
        init = true;
    }

    /* Find the minimal length L with cumulative count >= n */
    idx_t cumulative = 0;
    int L = 1;
    while (L < MAXLEN && cumulative + lenCount[L] < n) {
        cumulative += lenCount[L];
        ++L;
    }
    /* For very large n the above loop should always terminate because
       MAXLEN is chosen sufficiently large. */

    /* Position of the desired number within all numbers of length L (0‑based) */
    idx_t idx = n - cumulative - (idx_t)1;

    /* Construct the number digit by digit */
    seq_t value = 0;
    int prev = -1;   /* sentinel for "no previous digit" */
    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;   /* leading zero forbidden */
        for (int d = start; d < 5; ++d) {
            if (pos == 0 || ((d > prev) ? (d - prev) : (prev - d)) <= 1) {
                idx_t rem = dpLen[L - 1 - pos][d];
                if (idx < rem) {           /* choose this digit */
                    value = value * ((seq_t)5) + (seq_t)d;
                    prev  = d;
                    break;
                } else {
                    idx -= rem;
                }
            }
        }
    }
    return value;
}
