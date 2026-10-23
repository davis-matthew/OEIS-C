/*
 * A019463  Add 1, multiply by 1, add 2, multiply by 2, etc., start with 1.
 * https://oeis.org/A019463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 41, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019463: alternating +k / *k operations starting from 1
 * a(0) = 1
 * For m ≥ 1:
 *   a(2m-1) = a(2m-2) + m
 *   a(2m)   = a(2m-1) * m
 * This algorithm implements the above recurrence iteratively.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t val = 1;        /* current value of the sequence */
    seq_t k   = 1;        /* current parameter k for add/multiply */
    idx_t m = n >> 1;     /* number of complete (add, multiply) pairs */

    for (idx_t i = 0; i < m; ++i) {
        val += k;     /* odd operation: add k */
        val *= k;     /* even operation: multiply by k */
        ++k;          /* next k for the following pair */
    }

    if (n & 1)          /* if the last operation is an addition */
        val += k;

    return val;
}
