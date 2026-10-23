/*
 * A125714  Alfred Moessner's factorial triangle.
 * https://oeis.org/A125714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(j+1, j-i) where * j = A003056(n-1)+1, * t = j*(j+1)/2,
 *            * i = (t - n mod t) mod t, * S is the unsigned Stirling number
 *            of the first kind: * S(0,0)=1, S(m,0)=0,
 *            S(m,k)=S(m-1,k-1)+(m-1)*S(m-1,k)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

enum { STIRLING_MAX = 200 };           /* enough for n up to 10 000 */

static seq_t stirling[STIRLING_MAX+2][STIRLING_MAX+2];
static idx_t stirling_n_max = 0;

/* Precompute unsigned Stirling numbers S(m,k) for 0 ≤ m ≤ n. */
static void ensure_stirling(idx_t n)
{
    if (n <= stirling_n_max) return;
    if (n > STIRLING_MAX) n = STIRLING_MAX;

    stirling[0][0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        stirling[m][0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = stirling[m-1][k-1];
            seq_t term2 = (seq_t)(m-1) * stirling[m-1][k];
            stirling[m][k] = term1 + term2;
        }
    }
    stirling_n_max = n;
}

/* Largest r with r*(r+1)/2 ≤ m (m ≥ 0). */
static idx_t triangular_index(idx_t m)
{
    idx_t lo = 0, hi = 1;
    while (hi * (hi + 1) / 2 <= m) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 <= m)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1 – unused */

    idx_t m = n - 1;                   /* argument for A003056 */
    idx_t j = triangular_index(m) + 1;
    idx_t t = j * (j + 1) / 2;          /* period */
    idx_t rem = n % t;
    idx_t i = (t - rem) % t;           /* 0 ≤ i < t */
    idx_t k = j - i;                   /* column index */

    idx_t need = j + 1;                /* we need S(need,k) */
    if (k > need) return 0;            /* safety: should not occur */

    ensure_stirling(need);
    return stirling[need][k];
}
