/*
 * A008802  Molien series for group [2,9]+ = 229.
 * https://oeis.org/A008802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) + (n>=10 ? d(n-10) : 0) where d(m) =
 *            sum_{k=0..⌊m/9⌋} ((m-9k) is even ? (m-9k)/2 + 1 : 0)
 * Range:     u8 n <= 89, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   b_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_term.h"

/* Compute d(m) as defined above. */
static seq_t d_term(idx_t m)
{
    seq_t sum = 0;
    for (idx_t mult = 0; mult <= m; mult += 9) {
        idx_t j = m - mult;          /* j = m - 9k   */
        sum += b_term(j);
    }
    return sum;
}

/* Public entry point: a(n) for the OEIS sequence A008802. */
seq_t a(idx_t n)
{
    seq_t val = d_term(n);
    if (n >= 10)
        val += d_term(n - 10);
    return val;
}
