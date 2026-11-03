/*
 * A018881  Smallest tenth power that begins with n.
 * https://oeis.org/A018881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest tenth power that begins with n. Computed by
 *            brute‑force search of increasing bases m, extracting the first d
 *            decimal digits of m^10 and comparing to the index n. This
 *            algorithm is efficient for n up to 10 000 because the expected
 *            base m is < 200 on average
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 18, u128 n <= 919,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 8722 (2026-11-01)
 * Helpers:   ipow, num_digits, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/num_digits.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case – by definition a(0) = 0. */
    if (n == 0)
        return 0;

    /* Number of decimal digits of the index n. */
    int d = num_digits_idx(n);

    /* Search base m starting from 1 upwards. */
    for (idx_t m = 1; ; ++m) {
        /* Compute m^10 exactly. */
        seq_t val = ipow((seq_t)m, (seq_t)10);

        /* Determine the first d digits of val. */
        int len = num_digits(val);          /* number of digits of val */
        seq_t leading;
        if (len <= d) {
            /* If value has fewer than d digits, the whole value is the prefix. */
            leading = val;
        } else {
            /* Remove the trailing digits to leave only the first d digits. */
            seq_t div = pow10((idx_t)(len - d));
            leading = val / div;
        }

        /* Compare with the index n (cast to seq_t for comparison). */
        if (leading == (seq_t)n)
            return val;   /* Found the smallest base – return its 10th power. */
    }
}
