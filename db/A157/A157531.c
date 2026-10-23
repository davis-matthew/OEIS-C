/*
 * A157531  Triangle T(n, k) = binomial(2*n, n) + binomial(n, k)^2, read by
 *          rows.
 * https://oeis.org/A157531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where the triangle T(n,k) = binomial(2*n, n) +
 *            binomial(n,k)^2 is read by rows: row 0 → 0 elements, row 1 → 1
 *            element, etc
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility GCD for seq_t values */
static seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Exact binomial coefficient binom(n,k) using cancellation.
   Works for arbitrary precision seq_t and avoids overflow on 64‑bit builds. */
static seq_t _binom_seq(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;   /* numerator */
        seq_t den = i;           /* denominator */

        seq_t g1 = _gcd_seq(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = _gcd_seq(result, den);
        result /= g2;
        den /= g2;

        result *= num;
        if (den != 1) result /= den;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Use 128‑bit arithmetic to avoid overflow for very large n. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Offset of the first element of row r. */
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute T(r,k) = binom(2r,r) + binom(r,k)^2. */
    seq_t top  = _binom_seq(2 * r, r);
    seq_t mid  = _binom_seq(r, k);
    return top + mid * mid;
}
