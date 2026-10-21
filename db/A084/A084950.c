/*
 * A084950  Array of coefficients of denominator polynomials of the n-th
 *          approximation of the continued fraction x/(1+x/(2+x/(3+...,
 *          related to Laguerre polynomial coefficients.
 * https://oeis.org/A084950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the flattened rows of the triangle a(r,m) = ((r‑m)!/m!)
 *            * binomial(r‑m , m). a(n) is obtained by listing rows in
 *            increasing r and within each row increasing m (starting at 0)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 48, u64 n <= 120,
 *            u128 n <= 323, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to compute the number of entries in rows 0…r inclusive */
static inline idx_t cumulative(idx_t r)
{
    /* rows come in even/odd patterns:
       r = 2k : (k+1)^2
       r = 2k+1 : (k+1)(k+2)  */
    if ((r & 1) == 0) {               /* even r = 2k */
        idx_t k = r >> 1;
        return (k + 1) * (k + 1);
    } else {                           /* odd r = 2k+1 */
        idx_t k = r >> 1;
        return (k + 1) * (k + 2);
    }
}

/* Find the row r such that cumulative(r‑1) ≤ n < cumulative(r) */
static inline idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (cumulative(r) <= n)
        ++r;
    return r;
}

/* Binomial coefficient binomial(n,k) for idx_t arguments,
   returned as seq_t.  Works for arbitrary‑precision seq_t
   as well as ordinary integer types. */
static inline seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Calculate a single coefficient a(r,m) */
static inline seq_t coeff(idx_t r, idx_t m)
{
    idx_t k = r - m;                 /* k = r-m */

    /* binomial part */
    seq_t bin = binom_seq(k, m);

    /* product part: (m+1)…k = (n-m)! / m! */
    seq_t prod = 1;
    for (idx_t i = m + 1; i <= k; ++i)
        prod *= (seq_t)i;

    return bin * prod;
}

/* Public function: flattened sequence a(n) */
seq_t a(idx_t n)
{
    /* row and column indices */
    idx_t r = row_of(n);
    idx_t prev = (r == 0) ? 0 : cumulative(r - 1);
    idx_t c = n - prev;                  /* column index in row r */

    return coeff(r, c);
}
