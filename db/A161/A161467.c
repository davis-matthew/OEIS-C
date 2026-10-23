/*
 * A161467  Digital sum of n^3 is 3*(digital sum of n).
 * https://oeis.org/A161467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer m such that digit_sum(m^3) == 3*digit_sum(m) */
#include "../utils/digit_sum.h"

/* Returns the n-th term (1‑based).  The function works for any order of calls. */
seq_t a(idx_t n)
{
    idx_t m = 1;          /* current integer to test */
    idx_t count = 0;      /* how many valid numbers found so far */

    while (1) {
        seq_t m_s  = (seq_t)m;          /* convert to seq_t for arithmetic */
        seq_t ds_m = digit_sum(m_s);    /* sum of digits of m */
        seq_t cube = m_s * m_s * m_s;   /* m^3 */
        seq_t ds_cube = digit_sum(cube);/* sum of digits of m^3 */

        if (ds_cube == ds_m * (seq_t)3) {
            ++count;
            if (count == n)
                return m_s;            /* found the n-th term */
        }
        ++m;
    }
}
