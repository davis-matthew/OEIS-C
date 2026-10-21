/*
 * A218890  T(n,k) = ((n + k - 1)*(n + k - 2) - (-1 + (-1)^floor((n + k)/2))*n
 *          + (1 +(-1)^floor((n + k)/2))*k)/2; n , k > 0, read by
 *          antidiagonals.
 * https://oeis.org/A218890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where T(i,k) = ((i + k - 1)*(i + k - 2) - (-1 +
 *            (-1)^floor((i + k)/2))*i + (1 + (-1)^floor((i + k)/2))*k)/2. The
 *            sequence is read by antidiagonals from top to bottom
 * Range:     u8 n <= 252, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t triangular(idx_t d)
{
#if defined(__GNUC__) || defined(__clang__)
    return (idx_t)(((__uint128_t)d * (d + 1)) / 2);
#else
    idx_t a = d;
    idx_t b = d + 1;
    if (a & 1) a >>= 1; else b >>= 1;
    return a * b;
#endif
}

static inline idx_t find_diagonal(idx_t n)
{
    /* Find smallest d s.t. triangular(d) >= n. */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

static inline seq_t compute_T(seq_t i, seq_t k)
{
    seq_t s = i + k;                     /* i + k  >= 2 */
    int odd = (int)((s >> 1) & 1);       /* floor((i+k)/2) odd? */
    seq_t part1 = (s - 1) * (s - 2);     /* (i+k-1)*(i+k-2) */
    seq_t num = part1;
    if (odd)                 /* (-1)^e = -1 -> add 2*i  */
        num += (seq_t)2 * i;
    else                     /* (-1)^e = +1 -> add 2*k  */
        num += (seq_t)2 * k;
    return num / 2;
}

seq_t a(idx_t n)
{
    idx_t diag = find_diagonal(n);       /* diagonal index d */
    idx_t prev = triangular(diag - 1);   /* elements before this diagonal */
    idx_t pos  = n - prev - 1;           /* 0‑based position in the diagonal */

    seq_t i = (seq_t)(pos + 1);
    seq_t k = (seq_t)(diag - pos);

    return compute_T(i, k);
}
