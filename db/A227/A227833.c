/*
 * A227833  3-adic valuation of A005130(n).
 * https://oeis.org/A227833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(n) = a(n-1) + (s(2*n-2) + s(2*n-1) - s(n-1) -
 *            s(3*n-2)) / 2 (n ≥ 1) * where s(n) is the sum of the base‑3
 *            digits of n (A053735). The function * returns the value of
 *            A227833 at index n. It works for any index * order and any seq_t
 *            (built‑in integer or arbitrary‑precision type)
 * Range:     u8 n <= 978, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum3digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sum3digits.h"

/* ----- compute a(n) --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t val = 0;          /* running value of a(k) for the loop */

    for (idx_t k = 1; k <= n; ++k) {
        int s1 = sum3digits(2 * k - 2);
        int s2 = sum3digits(2 * k - 1);
        int s3 = sum3digits(k - 1);
        int s4 = sum3digits(3 * k - 2);

        int delta = (s1 + s2 - s3 - s4) / 2;           /* always integer */
        val = val + (seq_t)delta;
    }
    return val;
}
