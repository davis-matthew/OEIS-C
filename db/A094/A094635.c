/*
 * A094635  Smallest digit of n in Roman numeral representation.
 * https://oeis.org/A094635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 499, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determine the smallest Roman numeral value that appears in the
   representation of n.  The function inspects each decimal digit
   of n and, according to the Roman‐numeral rules, identifies the
   smallest symbol contributing to the representation.  No actual
   string construction is performed, so the routine runs in O(log10 n). */
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t best = 0;
    int first = 1;          /* flag for first encountered symbol */
    idx_t temp = n;
    int place = 0;          /* 0 = ones, 1 = tens, 2 = hundreds … */

    while (temp > 0) {
        idx_t d = temp % 10;   /* current decimal digit */
        temp /= 10;

        if (d != 0) {
            seq_t val;
            if (place == 0) {          /* ones */
                val = (d == 5) ? (seq_t)5 : (seq_t)1;
            } else if (place == 1) {   /* tens */
                val = (d == 5) ? (seq_t)50 : (seq_t)10;
            } else if (place == 2) {   /* hundreds */
                val = (d == 5) ? (seq_t)500 : (seq_t)100;
            } else {                    /* thousands and above */
                val = (seq_t)1000;
            }

            if (first || val < best) {
                best = val;
                first = 0;
                if (best == (seq_t)1)          /* cannot be smaller */
                    return best;
            }
        }
        ++place;
    }

    /* For n==0 (not required by the problem) return 0 */
    return best;
}
