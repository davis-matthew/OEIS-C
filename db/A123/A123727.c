/*
 * A123727  An example of a stereogram: a flat picture that appears
 *          three-dimensional when viewed in the correct way.
 * https://oeis.org/A123727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let m = ⌊√(n–1)⌋, center = m² + m + 1, k = |n – center|. a(n) =
 *            1 if k = 0 111 if k = 1 palindrome 11 2 3…k (k–1)…3 2 11 if k ≥
 *            2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t integer_sqrt(idx_t x)
{
    if (x < 2) return x;
    idx_t low = 1, high = 4294967295ull, mid, m = 1;   /* (1ULL<<32)-1 */
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid > x / mid) {
            high = mid - 1;
        } else {
            m = mid;
            low = mid + 1;
        }
    }
    return m;
}

seq_t a(idx_t n)
{
    idx_t m = integer_sqrt(n - 1);
    idx_t center = m * m + m + 1;
    idx_t k = (n >= center) ? (n - center) : (center - n);

    if (k == 0) return (seq_t)1;
    if (k == 1) return (seq_t)111;

    seq_t res = 11;
    for (idx_t i = 2; i <= k; ++i) {
        res = res * 10 + i;
    }
    for (idx_t i = k - 1; i > 1; --i) {
        res = res * 10 + i;
    }
    res = res * 10 + 1;
    res = res * 10 + 1;
    return res;
}
