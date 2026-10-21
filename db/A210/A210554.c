/*
 * A210554  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208341; see the Formula section.
 * https://oeis.org/A210554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals the n‑th element in the triangle of coefficients of
 *            v(n,x): T(r,k) = Σ_{i=1..k} binom(k-1,i-1) * binom(r-k+i,i)
 *            where r is the smallest integer with r(r+1)/2 ≥ n and k = n –
 *            (r-1)r/2
 * Range:     u8 n <= 31, u16 n <= 99, u32 n <= 340, u64 n <= 1208,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_idx(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r: smallest integer with r(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;          /* column index (1‑based) */

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t A = binom_idx(k - 1, i - 1);
        seq_t B = binom_idx(r - k + i, i);
        sum += A * B;
    }
    return sum;
}
