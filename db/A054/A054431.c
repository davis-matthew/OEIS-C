/*
 * A054431  Array read by antidiagonals: T(x, y) tells whether (x, y) are
 *          coprime (1) or not (0).
 * https://oeis.org/A054431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if gcd(i,j)=1 where (i,j) is the n‑th pair in the *
 *            antidiagonal enumeration, otherwise 0. * See OEIS A054431
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root via binary search (idx_t type). */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

/* Euclidean gcd for generic seq_t type. */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the offset of A054431 is 1. */

    /* --- Locate the diagonal (sum i+j = s).  --- */
    idx_t m = (idx_t)1 + (idx_t)8 * n;        /* 1 + 8*n   */
    idx_t sqrt_m = isqrt_idx(m);             /* floor(sqrt) */
    idx_t s = (idx_t)((1 + sqrt_m) / 2);     /* initial guess */
    idx_t tri = (s - 1) * s / (idx_t)2;      /* triangular number for s */
    if (tri < n) ++s;                        /* adjust to the right diagonal */

    /* --- Determine position k on this diagonal.  --- */
    idx_t total_prev = ((s - 2) * (s - 1)) / (idx_t)2;
    idx_t k = n - total_prev;                /* 1 ≤ k ≤ s-1 */

    /* Corresponding coordinates (i,j).  */
    seq_t i = (seq_t)k;
    seq_t j = (seq_t)(s - k);

    /* --- Return 1 if i and j are coprime, else 0.  --- */
    seq_t g = gcd_seq_t(i, j);
    return (g == (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
