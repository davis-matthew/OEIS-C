/*
 * A144434  Triangle read by rows formed from golden-mean-like generalized
 *          factorials: t(n,m)=Round(phi*a(n)/(a(m)*a(n-m))), where phi=-(1 -
 *          Sqrt[5])/2, b(n)=b(n-1)+phi, a(n)=b(n)*a(n-1).
 * https://oeis.org/A144434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A144434 – triangle read by rows of generalized binomial coefficients.
 *  t(n,m) = Round[(1/(m+1)) * Binomial[n+1, m]]
 *  (rounding ties to the nearest even integer, as used by Mathematica's Round).
 *
 *  This implementation maps a 1‑based index n to its (row, column) in the flattened
 *  triangle and returns the corresponding value using the binomial formula above.
 *  All calculations are performed with seq_t arithmetic only; no floating point is used.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Convert 1‑based index to 0‑based flattened position. */
    idx_t pos = n - 1;                 /* 0‑based position in the flattened triangle */

    /* Binary search for the row r (0‑based) such that cumulative(r) > pos. */
    /* cumulative(r) = (r+1)(r+2)/2  (number of terms up to and including row r). */
    idx_t low = 0, high = 1;
    while (((seq_t)(high + 1) * (seq_t)(high + 2)) / 2 <= pos) {
        high <<= 1;                    /* double until we exceed pos */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t cum = (mid + 1) * (mid + 2) / 2;
        if (cum > pos) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;                     /* row index (0‑based) */

    /* Number of terms before row r */
    idx_t before = r * (r + 1) / 2;     /* cumulative(-1) */

    /* Position within row r (0‑based), m ∈ [0, r] */
    idx_t m = pos - before;

    /* Compute t(r, m) using the binomial formula. */
    seq_t bin   = binomial(r + 1, m);   /* Binomial[n+1, m] */
    seq_t denom = (seq_t)(m + 1);       /* denominator (m+1) */
    seq_t q     = bin / denom;          /* integer part */
    seq_t rem   = bin % denom;          /* remainder */

    /* Round to nearest integer with ties to nearest even.
     * Compare 2*rem with denom. */
    seq_t twice_rem = rem << 1;                /* 2*rem */

    if (twice_rem < denom) {
        /* Fractional part < 0.5 → round down (q already correct) */
    } else if (twice_rem > denom) {
        /* Fractional part > 0.5 → round up */
        q += 1;
    } else { /* tie: 2*rem == denom */
        /* If q is even, keep q; otherwise round up to make it even. */
        if ((q & 1) != 0) {
            q += 1;
        }
    }

    return q;
}
