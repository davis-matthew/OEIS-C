/*
 * A133995  Irregular array read by rows: n-th row contains (in numerical
 *          order) the positive integers <= n which are neither divisors of n
 *          nor are coprime to n. A 0 is put into row n if there are no such
 *          integers.
 * https://oeis.org/A133995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) reads the irregular array by rows defined for OEIS A133995.
 *            * The array is constructed as follows: * - Row k contains, in
 *            increasing order, all positive integers i ≤ k that * are neither
 *            a divisor of k nor coprime to k. * - If k has no such integers,
 *            a 0 is written for that row. * The sequen...
 * Range:     u8 n <= 9666, u16 n <= 9666, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean GCD for idx_t ----------------------------------------- */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the a(n) value for a given 1‑based index n ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but safe for completeness */
        return (seq_t)0;

    /* target is the remaining position we are looking for in the flattened
       array (1‑based). */
    idx_t target = n;

    /* We iterate over rows until the target falls into the current row. */
    for (idx_t row = 1; ; ++row) {
        /* Count the special numbers in this row. */
        idx_t specials = 0;
        for (idx_t x = 1; x <= row; ++x) {
            if (row % x == 0)          /* divisor of row – skip */
                continue;
            if (gcd_idx_t(row, x) == 1) /* coprime – skip */
                continue;
            ++specials;
        }

        if (specials == 0) {               /* row contains a single 0 */
            if (target == 1)
                return (seq_t)0;
            target--;                        /* consume the 0 of this row */
            continue;
        }

        if (target > specials) {           /* skip this whole row */
            target -= specials;
            continue;
        }

        /* The desired element lies in this row.  Find the target‑th element. */
        idx_t found = 0;
        for (idx_t x = 1; x <= row; ++x) {
            if (row % x == 0)
                continue;
            if (gcd_idx_t(row, x) == 1)
                continue;
            ++found;
            if (found == target)
                return (seq_t)x;
        }
        /* The loop above is guaranteed to return.  Return 0 as fail‑safe. */
        return (seq_t)0;
    }
}
