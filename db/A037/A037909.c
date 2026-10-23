/*
 * A037909  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*6^i is the base-6 representation of n.
 * https://oeis.org/A037909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ i | 1 <= i <= m , |d(i)-d(i-1)| = 1 }| where n =
 *            Σ_{k=0..m} d(k) * 6^k is the base‑6 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    seq_t count = 0;

    /* process the least significant digit first */
    if (tmp == 0) return count;   /* defensive: for n = 0 */

    idx_t prev = tmp % 6;
    tmp /= 6;

    while (tmp > 0) {
        idx_t curr = tmp % 6;
        idx_t diff = (curr > prev) ? (curr - prev) : (prev - curr);
        if (diff == 1) count = count + 1;  /* seq_t addition */
        prev = curr;
        tmp /= 6;
    }

    return count;
}
