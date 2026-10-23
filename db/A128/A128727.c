/*
 * A128727  Triangle read by rows: T(n,k) is the number of skew Dyck paths of
 *          semilength n having k DDU and LDU's.
 * https://oeis.org/A128727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the triangle entry T(r,k) read by rows. T(r,k) =
 *            (1/r)*3^(r-1-2k)*binomial(r,k)*binomial(r-k,k+1) Row 0 has 1
 *            entry T(0,0)=1
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities used:
   - binomial (idx_t a, idx_t b)          Compute binomial coefficient.
   - pow3 (idx_t exp)                     Return 3^exp. */
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Special case for the first entry. */
    if (n == 0) return (seq_t)1;

    /* Find the row r and local column k for the index n. */
    idx_t r = 0;
    idx_t pos = 0;                     /* number of entries before row r */
    for (;;) {
        idx_t len = (r == 0) ? 1 : (r + 1) / 2;   /* length of row r */
        if (pos + len > n) break;
        pos += len;
        ++r;
    }
    idx_t k = n - pos;                 /* column within the row */

    /* Compute T(r,k) via the closed‑form formula. */
    seq_t result = binomial(r, k);
    result *= binomial(r - k, k + 1);

    idx_t exp = r - 1;                 /* exp = r-1-2k */
    exp -= 2 * k;
    result *= pow3(exp);

    result /= r;                       /* divide by r (r>0 by construction) */
    return result;
}
