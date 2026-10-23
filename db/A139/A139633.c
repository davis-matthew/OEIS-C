/*
 * A139633  Triangle read by rows: binomial transform of a diagonalized matrix
 *          of A026741.
 * https://oeis.org/A139633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r and c be the row and column (1‑based) of the * triangle
 *            entry that is read in row‑major order. * Then a(n) =
 *            binomial(r-1, c-1) * f(c) where * f(c) = c if c is odd, * c/2 if
 *            c is even. * The triangle is the binomial transform of the
 *            diagonal matrix * with diagonal entries A026741: A026741(k...
 * Range:     u8 n <= 40, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t a, idx_t b) {
    if (b > a) return 0;
    idx_t k = b;
    if (k > a - k) k = a - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(a - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* for safety; sequence is defined from n>=1 */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index c (1‑based) */
    idx_t prev = (r * (r - 1)) / 2;    /* number of terms before this row */
    idx_t c = n - prev;                /* 1 <= c <= r */

    /* Diagonal value for column c: A026741(c) */
    seq_t diag = (c & 1) ? (seq_t)c : (seq_t)(c / 2);

    /* Binomial coefficient C(r-1, c-1) */
    seq_t bin = binom(r - 1, c - 1);

    return bin * diag;
}
