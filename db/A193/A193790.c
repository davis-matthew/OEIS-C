/*
 * A193790  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(2x+1)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(n - r*(r+1)/2) in (2x+1)^(r-1) when n
 *            lies in row r of the fused triangle, or a(n) = 3^(r-1) if it is
 *            the last entry of that row. r is the unique integer with
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2
 * Range:     u8 n <= 34, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle row 0 explicitly */
    if (n == 0) return 1;

    /* Find r with binary search: r*(r+1)/2 <= n */
    idx_t low = 0, high = n + 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute start index of row r */
    unsigned __int128 tmp = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)tmp;

    /* Column within the row */
    idx_t c = n - start;

    /* Last element of the row */
    if (c == r) {
        /* Compute 3^(r-1) by exponentiation by squaring */
        seq_t res = 1;
        seq_t base = 3;
        idx_t exp = r - 1;
        while (exp) {
            if (exp & 1) res *= base;
            base *= base;
            exp >>= 1;
        }
        return res;
    } else {
        /* Coefficient of x^c in (2x+1)^(r-1) */
        seq_t res = 1;
        idx_t N = r - 1;        /* degree of the polynomial */
        idx_t k = c;
        if (k > N - k) k = N - k; /* use symmetry to minimize work */
        /* Compute binomial(N, k) */
        for (idx_t i = 1; i <= k; ++i) {
            res = res * (N - k + i) / i;
        }
        /* Multiply by 2^c via left shift */
        res <<= c;
        return res;
    }
}
