/*
 * A135839  Triangle read by rows: starting with A128174, replace left border
 *          with (1, 1, 1, ...).
 * https://oeis.org/A135839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n - r(r-1)/2. T(r,1) = T(r,r) = 1; otherwise T(r,k) = 1 if r%2
 *            == k%2, else 0
 * Range:     u8 n <= 9821, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* Off‑by‑one guard (offset 1) */

    idx_t low = 1, high = 1;
    /* Find an upper bound for the row using exponential search */
    while ((__uint128_t)high * (high + 1) / 2 < n) high <<= 1;

    /* Binary search for the exact row r such that r(r+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t s = (__uint128_t)mid * (mid + 1) / 2;
        if (s >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Compute the index of the first element of row r */
    __uint128_t prev128 = (__uint128_t)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;
    idx_t k = n - prev;   /* 1‑based column within the row */

    /* Border elements are always 1 */
    if (k == 1 || k == r) return (seq_t)1;

    /* Interior: value is 1 iff r and k have the same parity */
    if ((r & 1) == (k & 1)) return (seq_t)1;
    return (seq_t)0;
}
