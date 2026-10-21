/*
 * A083003  Triangle read by rows in which each term represents the total area
 *          of all the squares found in the grid described in sequence
 *          A082652.
 * https://oeis.org/A083003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle entries are given by T(l,c) = (5*l*c^4 + 20*l*c^3 +
 *            25*l*c^2 + 10*l*c + 10*c^2 + 8*c - 3*c^5 - 10*c^4 - 5*c^3) / 60
 *            The sequence a(n) lists T(l,c) read by rows (row l has l
 *            columns), where n = 1,2,3,
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          // undefined for n = 0, return 0

    /* Find the row l such that l*(l+1)/2 >= n using binary search. */
    idx_t low = 1, high = 1;
    /* Increase high until it is large enough. */
    while (((unsigned __int128)high * (high + 1)) / 2 < n) {
        high <<= 1;
        if (high == 0) {          /* overflow, cap at max value */
            high = ~((idx_t)0);
            break;
        }
    }
    /* Binary search for minimal l with triangular number >= n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri >= n) high = mid;
        else          low = mid + 1;
    }
    idx_t l = low;
    /* Previous total count before row l: (l-1)*l/2 */
    unsigned __int128 prev = ((unsigned __int128)(l - 1) * l) / 2;
    idx_t c = n - (idx_t)prev;    /* column index (1 ≤ c ≤ l) */

    /* Compute the polynomial using seq_t arithmetic. */
    seq_t sl = (seq_t)l;
    seq_t sc = (seq_t)c;
    seq_t c2 = sc * sc;
    seq_t c3 = c2 * sc;
    seq_t c4 = c3 * sc;
    seq_t c5 = c4 * sc;

    seq_t num =
        (seq_t)5 * sl * c4
      + (seq_t)20 * sl * c3
      + (seq_t)25 * sl * c2
      + (seq_t)10 * sl * sc
      + (seq_t)10 * c2
      + (seq_t)8 * sc;

    /* Subtract the negative terms (sum is known to be non‑negative). */
    num -= (seq_t)3 * c5;
    num -= (seq_t)10 * c4;
    num -= (seq_t)5 * c3;

    /* Divide by 60 to obtain the final integer value. */
    return num / (seq_t)60;
}
