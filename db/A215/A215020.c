/*
 * A215020  a(n) = log_2( A182105(n) ).
 * https://oeis.org/A215020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A082850(n) - 1, where A082850(n) is the length of the
 *            k‑th irregular row (0 … ruler(k)), hence a(n) is the offset
 *            within the row (0‑based)
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* prefix sum of row lengths */
    idx_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* compute the ruler function r(k) = exponent of 2 in k */
        idx_t r = 0;
        idx_t m = k;
        while ((m & 1) == 0) {
            ++r;
            m >>= 1;
        }

        idx_t row_len = r + 1;           /* values 0 … r */
        if (sum + row_len >= n) {        /* n lies in this row */
            idx_t offset = n - sum - 1;  /* 0‑based offset */
            return (seq_t)offset;
        }
        sum += row_len;
    }
}
