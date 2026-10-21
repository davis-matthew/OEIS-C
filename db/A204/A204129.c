/*
 * A204129  Symmetric matrix based on f(i,j)=(L(i) if i=j and 1 otherwise),
 *          where L=A000032 (Lucas numbers), by antidiagonals.
 * https://oeis.org/A204129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 unless the nth term in antidiagonal order lies on the
 *            main diagonal, in which case a(n) = L(i), the i‑th Lucas number
 *            (L(1)=1, L(2)=3, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _lucas(idx_t k) {
    if (k == 1) return (seq_t)1;
    if (k == 2) return (seq_t)3;
    seq_t a = (seq_t)1;
    seq_t b = (seq_t)3;
    for (idx_t i = 3; i <= k; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* offset starts at 1 */
    idx_t s = 2;
    idx_t sum = 0;
    while (1) {
        sum += s - 1;
        if (sum >= n) break;
        ++s;
    }
    idx_t start = sum - (s - 1) + 1;      /* first index of diagonal s */
    idx_t p = n - start;                 /* position within the diagonal (0‑based) */
    idx_t i = p + 1;
    idx_t j = s - i;
    if (i == j) {
        return _lucas(i);
    } else {
        return (seq_t)1;
    }
}
