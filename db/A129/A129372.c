/*
 * A129372  Triangle read by rows: T(n,k) = 1 if k divides n and n/k is odd,
 *          T(n,k) = 0 otherwise.
 * https://oeis.org/A129372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the unique row–column pair (r,k) with n = r(r−1)/2 +
 *            k satisfies k|r and (r/k) odd; otherwise 0
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = 1;
    /* find the row r such that r(r+1)/2 >= n */
    while ((seq_t)r * (r + 1) / 2 < n) ++r;

    idx_t prev = (r - 1) * r / 2;     /* last index of previous row */
    idx_t k = n - prev;               /* column (1‑based) */

    if (r % k == 0 && ((r / k) & 1)) {
        return (seq_t)1;
    }
    return (seq_t)0;
}
