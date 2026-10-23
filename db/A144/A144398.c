/*
 * A144398  Coefficients of a symmetrical polynomial set:( Pascal's triangle
 *          with central zeros) p(x,n)=If[n <= 4, Sum[Binomial[n, i]*x^i, {i,
 *          0, n}], x^n + n*x^(n - 1) + Binomial[n, 2]*x^(n - 2) + n*x +
 *          Binomial[n, 2]*x^2 + 1].
 * https://oeis.org/A144398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144398 – coefficients of the reordered polynomials
   p(x,n).  For each n ≥ 1 the sequence is obtained by
   listing the coefficients of p(x,n) in the order
   1 (x⁰), 1 (xⁿ), then the remaining coefficients for
   exponents 1 … n‑1 in natural order.
   The function returns the n‑th term of this flattened list. */

seq_t a(idx_t n) {
    /* --- locate the polynomial row r that contains term n --- */
    idx_t low = 1, high = 1;
    /* increase high until S(high) = high*(high+3)/2 ≥ n */
    while ((unsigned __int128)high * (high + 3) / 2 < n) {
        if (high > ((idx_t)~0) >> 1) {  /* avoid overflow of high << 1 */
            high = (idx_t)~0;
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 3) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;  /* r ≥ 1 */

    /* --- find position of n inside row r --- */
    unsigned __int128 s_prev = (unsigned __int128)(r - 1) * (r + 2) / 2; /* S(r-1) */
    unsigned __int128 off128 = (unsigned __int128)n - s_prev - 1;      /* 0‑based offset in this row */
    idx_t offset = (idx_t)off128;  /* offset < r+1, fits in idx_t */

    /* first two coefficients are always 1 */
    if (offset <= 1)
        return (seq_t)1;

    /* exponent corresponding to remaining coefficients */
    idx_t e = offset - 1;  /* e ranges 1 … r‑1 */

    /* compute the coefficient according to the definition:
       - e = 1 or r‑1  → coefficient r
       - e = 2 or r‑2  → coefficient binomial(r,2)
       - otherwise      → coefficient 0 */
    if (e == 1 || e == r - 1)
        return (seq_t)r;

    if (e == 2 || e == r - 2) {
        unsigned __int128 bin2 = (unsigned __int128)r * (r - 1) / 2;
        return (seq_t)bin2;
    }

    return (seq_t)0;
}
