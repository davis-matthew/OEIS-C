/*
 * A192020  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the binomial tree of order n (1 <= k <=
 *          2n-1; entries in row n are the coefficients of the corresponding
 *          Wiener polynomial).
 * https://oeis.org/A192020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Σ_{j=1}^{r} 2^{r-j} * C(2j-2, k-1) , where r = ⌊√n⌋ + 1
 *            (row number, 1‑based) and k = n - (r-1)² + 1 (column number,
 *            1‑based)
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root of an unsigned 64‑bit value */
static inline idx_t integer_sqrt(idx_t v)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);          /* > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;           /* upper mid to avoid infinite loop */
        if (mid <= v / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;                                   /* floor(√v) */
}

/* compute 2^e using only seq_t operations (no shift) */
static inline seq_t pow2_u(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) r = r * (seq_t)2;
    return r;
}

/* multiplicative binomial coefficient: C(n,k) */
static inline seq_t binom_u(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;                     /* symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;                     /* exact division */
    }
    return res;
}

/* OEIS sequence A192020: flattened binomial‑tree distances */
seq_t a(idx_t n)
{
    /* Determine row r (1‑based) and column k (1‑based) */
    idx_t r = integer_sqrt(n) + 1;                /* r = ⌊√n⌋ + 1 */
    idx_t offset = (r - 1) * (r - 1);             /* first index of row r */
    idx_t k_minus1 = n - offset;                 /* 0‑based column index */

    seq_t sum = (seq_t)0;

    /* Sum over j = 1 .. r : 2^{r-j} * C(2j-2, k-1) */
    for (idx_t j = 1; j <= r; ++j) {
        idx_t binom_index = 2 * j - 2;             /* 0‑based top for binomial */
        if (k_minus1 > binom_index) continue;     /* binomial is zero */
        seq_t bin = binom_u(binom_index, k_minus1);
        seq_t pow = pow2_u(r - j);
        sum = sum + (bin * pow);
    }

    return sum;
}
