/*
 * A099091  Riordan array (1, 2+3x).
 * https://oeis.org/A099091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flattening of the Riordan triangle (1, 2+3x). For each n
 *            we find the row r (with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2) and
 *            column k = n - r*(r+1)/2. Then a(n) = T(r,k) = binomial(k, r-k)
 *            * 2^(2k-r) * 3^(r-k). (The T(r,k) are the entries of the Riordan
 *            array (1, 2+3x).)
 * Range:     u8 n <= 32, u16 n <= 85, u32 n <= 268, u64 n <= 975,
 *            u128 n <= 3546, big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* start index of this row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column within the row */

    idx_t r_minus_k = r - k;
    if (r_minus_k > k) {
        /* binomial(k, r-k) is zero, hence the entry is zero */
        return (seq_t)0;
    }

    seq_t val = binomial(k, r_minus_k);

    /* exponents for powers of 2 and 3 */
    idx_t exp2 = 2 * k - r;   /* always non‑negative because k ≥ r-k */
    idx_t exp3 = r_minus_k;   /* r-k ≥ 0 */

    /* multiply by the required powers */
    val = val * pow2(exp2) * pow3(exp3);
    return val;
}
