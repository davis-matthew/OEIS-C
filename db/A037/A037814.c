/*
 * A037814  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*7^i
 *          is the base-7 representation of n.
 * https://oeis.org/A037814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i ≥ 1 such that d(i) ≤ d(i‑1) in the
 *            base‑7 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t tmp = n;
    seq_t cnt = 0;

    if (tmp == 0)   /* the offset starts at n = 1; this case is safe */
        return 0;

    idx_t prev = tmp % 7;   /* least significant digit */
    tmp /= 7;

    while (tmp > 0) {
        idx_t cur = tmp % 7;   /* next less significant digit */
        if (cur <= prev) ++cnt;
        prev = cur;
        tmp /= 7;
    }

    return cnt;
}
