/*
 * A161982  Numbers with base-16 digits only from the set {6,8,9,12=C}.
 * https://oeis.org/A161982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose base‑16 representation involves
 *            only the digits 6, 8, 9 and C (12). The algorithm finds the
 *            digit length m necessary for the n-th term, then interprets (n −
 *            1) in base‑4 with m digits, maps each base‑4 digit to
 *            {6,8,9,12}, and evaluates the resulting base‑16 number
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* determine digit length m such that sum_{k=1}^m 4^k >= n */
    idx_t pow4 = 4;          /* current 4^m (starts with 4^1) */
    idx_t cumulative = pow4;/* sum of 4^1 .. 4^m so far */
    idx_t cumulative_prev = 0;/* sum of 4^1 .. 4^{m-1} */
    idx_t m = 1;

    while (n > cumulative) {
        cumulative_prev = cumulative;
        pow4 = pow4 * 4;        /* grow to next power of 4 */
        cumulative = cumulative + pow4;
        m = m + 1;
    }

    /* offset within numbers of length m (0‑based) */
    idx_t offset = n - cumulative_prev - 1;

    /* mapping from base‑4 digit to allowed hex digit */
    static const seq_t map[4] = {6, 8, 9, 12};

    seq_t value = 0;
    seq_t weight = 1;   /* current 16^i */

    for (idx_t i = 0; i < m; ++i) {
        idx_t digit = offset % 4;   /* 0..3 */
        offset = offset / 4;
        value = value + (map[digit] * weight);
        weight = weight * 16;
    }

    return value;
}
