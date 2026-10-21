/*
 * A231714  Lower triangular region of A231713; A triangular table read by
 *          rows: T(n,k) = sum of absolute values of digit differences in the
 *          factorial base representations (A007623) of n and k, where (n, k)
 *          = (0,0), (1,0), (1,1), (2,0), (2,1), (2,2), ..., n >= 0 and (0 <=
 *          k <= n).
 * https://oeis.org/A231714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = sum_{i≥1} |d_i(r) – d_i(c)| where r = ⌊(√(8n+1) – 1)/2⌋,
 *            c = n – r(r+1)/2 and d_i(x) is the i‑th factorial base digit of
 *            integer x (i.e. the remainder when dividing repeatedly by
 *            2,3,4,…)
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t row_from_index(idx_t idx)
{
    idx_t low = 0, high = idx, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= idx) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row (r) and column (c) for the
       lower triangular flattening of T(n,k). */
    idx_t r = row_from_index(n);
    idx_t prev = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t c = n - prev;

    /* Sum of absolute differences of factorial base digits. */
    seq_t sum = 0;
    idx_t a_val = r, b_val = c;
    idx_t divisor = 2;                    /* factoradic base starts at 2 (=1!) */

    while (a_val > 0 || b_val > 0) {
        idx_t d_a = a_val % divisor;
        a_val /= divisor;
        idx_t d_b = b_val % divisor;
        b_val /= divisor;
        seq_t diff = (d_a > d_b) ? (seq_t)(d_a - d_b)
                                 : (seq_t)(d_b - d_a);
        sum += diff;
        ++divisor;
    }
    return sum;
}
