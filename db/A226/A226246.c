/*
 * A226246  Triangle T(n, k), read by rows 1<=n, 1<=k<=n: Number of cells
 *          touched by a unit-width diagonal in a regular n X k grid.
 * https://oeis.org/A226246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,column) where T(n,k) = n + k + g * + 2*(
 *            g*floor(r/g) - floor(r/min(n,k)) - 1 ), * r = sqrt(n*n + k*k)/2,
 *            g = gcd(n,k). * The sequence is the flattened triangle read by
 *            rows (offset 1)
 * Range:     u8 n <= 2924, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t bit = 1ULL << 62;               /* highest power of 4 ≤ x */
    while (bit > x) bit >>= 2;
    idx_t res = 0;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) | bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, undefined for 0 */

    /* Determine the row r such that r(r+1)/2 ≥ n */
    idx_t r;
    {
        idx_t s = isqrt_u64(8 * n + 1);
        r = (s - 1) / 2;
        if (r * (r + 1) / 2 < n) ++r;
    }

    /* Column in this row (1 ≤ c ≤ r) */
    idx_t sum_before = r * (r - 1) / 2;
    idx_t c = n - sum_before;

    /* Core triangle formula */
    idx_t g   = gcd_u64(r, c);
    idx_t mn  = (r < c) ? r : c;
    idx_t rad = isqrt_u64(r * r + c * c);

    idx_t floor_r_over_g  = rad / (2 * g);
    idx_t floor_r_over_mn = rad / (2 * mn);

    idx_t val = r + c + g + 2 * (g * floor_r_over_g - floor_r_over_mn - 1);

    return (seq_t)val;
}
