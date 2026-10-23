/*
 * A176226  A symmetrical triangle sequence: T(n, k) = q^k + q^(n-k) - q^n,
 *          with q=5.
 * https://oeis.org/A176226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = 5^c + 5^(r-c) - 5^r, * where r = ⌊(⌊√(8n+1)⌋‑1)/2⌋ and c
 *            = n – r(r+1)/2
 * Range:     i8 n <= 10, i16 n <= 28, i32 n <= 105, i64 n <= 406,
 *            i128 n <= 1540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t low = 1, high = x / 2, mid, res = 1;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {
            low = mid + 1;
            res = mid;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

static seq_t pow5_u64(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    for (idx_t i = 0; i < exp; ++i) result *= base;
    return result;
}

seq_t a(idx_t n)
{
    idx_t t   = 8 * n + 1;          /* 8n + 1 */
    idx_t f   = isqrt_u64(t);       /* ⌊√(8n+1)⌋ */
    idx_t r   = (f - 1) >> 1;       /* floor((f-1)/2) */
    idx_t start = r * (r + 1) >> 1; /* start index of row r  = r(r+1)/2 */
    idx_t c   = n - start;          /* column within the row */

    seq_t p0  = pow5_u64(c);         /* 5^c          */
    seq_t p1  = pow5_u64(r - c);     /* 5^(r-c)      */
    seq_t pr  = pow5_u64(r);         /* 5^r          */

    return p0 + p1 - pr;             /* T(r,c)        */
}
