/*
 * A155890  a(n+1) is the least integer > a(n) containing all digits of a(n);
 *          a(1)=2.
 * https://oeis.org/A155890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 91, u64 n <= 362,
 *            u128 n <= 1483, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A155890: a(n+1) is the least integer > a(n) containing all digits of a(n);
 * a(1) = 2.
 *
 * The terms have only digits 0,1,2, and each length‑m term has one 1, one 2,
 * and (m-2) zeros, with the first digit non‑zero.  For m ≥ 2 there are
 * exactly 2 · (m−1) such numbers.  The sequence is simply these numbers
 * ordered increasingly by value.  This implementation constructs the
 * required term in O(√n) time.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n == 1) return (seq_t)2;

    /* Find the length m of the desired term.  The cumulative count
     * of terms up to length m is m² − m + 1.  Find the smallest m
     * with m² − m + 1 ≥ n. */
    idx_t m = 1;
    while ( ((seq_t)m * m - (seq_t)m + (seq_t)1) < n ) ++m;

    /* Count of terms with lengths less than m. */
    idx_t prev_len = m - 1;
    seq_t cumulative_prev =
        ((seq_t)prev_len * prev_len - (seq_t)prev_len + (seq_t)1);

    /* 0‑based index of the desired term within all terms of length m. */
    idx_t idx_inside = (idx_t)(n - cumulative_prev - (idx_t)1);

    /* Construct the term. */
    seq_t result;
    if (idx_inside < (m - 1)) {
        /* First digit is 1; the second non‑zero digit (2) is placed
         * from the rightmost possible position upwards. */
        seq_t first = (seq_t)1, other = (seq_t)2;
        idx_t pos = m - idx_inside;                 /* position of '2' (1‑based) */
        result = first * pow10(m - 1) + other * pow10(m - pos);
    } else {
        /* First digit is 2; the second non‑zero digit (1) is placed
         * from the rightmost possible position upwards. */
        idx_t k = idx_inside - (m - 1);
        seq_t first = (seq_t)2, other = (seq_t)1;
        idx_t pos = m - k;                           /* position of '1' (1‑based) */
        result = first * pow10(m - 1) + other * pow10(m - pos);
    }

    return result;
}
