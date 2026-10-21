/*
 * A229235  Smallest triangular numbers whose decimal expansion begins
 *          (nontrivially) with n-th triangular number.
 * https://oeis.org/A229235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest triangular number T_k (k > n) * that starts with
 *            the decimal representation of the n-th * triangular number T_n
 *            and is not followed by only zero digits
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 292, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* T_n */
    seq_t prefix = n_seq * (n_seq + 1) / 2;
    int dP = digit_len(prefix);          /* number of digits of the prefix */

    seq_t tri = prefix;                  /* current triangular number */
    idx_t k = n + 1;                     /* next index to test */

    while (true) {
        tri += k;                         /* T_k = T_{k-1} + k */
        ++k;

        int dT = digit_len(tri);
        if (dT <= dP)                     /* too short to have the prefix */
            continue;

        int shift = dT - dP;              /* digits of the suffix */
        seq_t divisor = pow10((idx_t)shift);
        seq_t lead = tri / divisor;       /* digits before the suffix */

        if (lead != prefix)
            continue;                    /* prefix does not match */

        seq_t rem = tri % divisor;        /* the suffix */

        /* the first digit after the prefix must be non‑zero:
           remainder must be at least 10^(shift‑1) */
        seq_t min_rem = pow10((idx_t)(shift - 1));
        if (rem < min_rem)
            continue;

        return tri;
    }
}
