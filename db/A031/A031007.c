/*
 * A031007  Triangle T(n,k): Write n in base 7, reverse order of digits, to
 *          get row n.
 * https://oeis.org/A031007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031007: a(n) is the nth term of the sequence obtained by writing every
   non‑negative integer in base 7, reversing the order of the digits of each
   representation, and then concatenating all rows.
*/

#include <stdint.h>          /* not required by the build, harmless */

static inline idx_t num_digits(idx_t x) {
    /* number of base‑7 digits of x; 0 has one digit */
    if (x == 0) return 1;
    idx_t d = 0;
    while (x > 0) {
        ++d;
        x /= 7;
    }
    return d;
}

seq_t a(idx_t n) {
    /* locate the row that contains the n-th element */
    idx_t rem = n;
    idx_t row = 0;
    while (1) {
        idx_t d = num_digits(row);
        if (rem < d) break;
        rem -= d;
        ++row;
    }

    /* extract the rem-th (0‑based) digit of the reversed base‑7 representation 
       of row.  This is the rem‑th least significant digit in base 7. */
    idx_t tmp = row;
    for (idx_t i = 0; i < rem; ++i) {
        tmp /= 7;
    }
    return (seq_t)(tmp % 7);
}
