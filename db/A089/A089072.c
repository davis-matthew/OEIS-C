/*
 * A089072  Triangle read by rows: T(n,k) = k^n, n >= 1, 1 <= k <= n.
 * https://oeis.org/A089072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k^r where r is the unique integer with r(r‑1)/2 < n ≤
 *            r(r+1)/2, * and k = n – r(r‑1)/2. Compute r by a simple linear
 *            scan (r is O(√n)) * and then evaluate k^r with exponentiation by
 *            squaring
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* below offset is undefined */
    /* Find the row r such that r(r‑1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;

    /* Compute column index k. */
    idx_t prev_total = r * (r - 1) / 2;
    idx_t k = n - prev_total;              /* 1 ≤ k ≤ r */

    /* Fast exponentiation: result = k^r. */
    seq_t base = (seq_t)k;
    seq_t result = (seq_t)1;
    idx_t exp = r;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
