/*
 * A057103  Triangle of congrua: T(n,k) = 4*n*k(n^2-k^2) with n>k>0 and
 *          starting at T(2,1) = 24. A055096(n)^2 + a(n) is a square, as is
 *          A055096(n)^2 - a(n).
 * https://oeis.org/A057103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4*i*k*(i*i - k*k) * where i and k are the row and column
 *            of the * triangle of congrua T(i,k) = 4*i*k*(i^2-k^2) *
 *            enumerated by increasing i and, for each i, * increasing k from
 *            1 to i-1. * The OEIS offset is 2, i.e. a(2) is the first * entry
 *            T(2,1)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned integers */
static unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long lo = 0, hi = x, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)       lo = mid + 1;
        else if (mid == 0) break;
        else              hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined for indices < 2 */

    unsigned long long idx = n;          /* work in unsigned 64‑bit */
    unsigned long long pos = idx - 2;    /* position starting at 0 */

    /* find row i: smallest i >= 2 with i*(i-1)/2 > pos */
    unsigned long long t = 1 + 8ULL * pos;
    unsigned long long s = isqrt64(t);
    unsigned long long row = (1 + s) / 2 + 1;   /* i */

    /* previous cumulative terms before this row */
    unsigned long long prev = (row - 2) * (row - 1) / 2;
    unsigned long long offset = pos - prev;      /* 0‑based within the row */
    unsigned long long col = offset + 1;         /* k = 1 … row-1 */

    /* compute 4*i*k*(i^2 - k^2) using seq_t arithmetic only */
    seq_t ii = (seq_t)row;
    seq_t kk = (seq_t)col;
    seq_t prod = ii * kk;            /* i*k */
    prod = 4 * prod;                 /* 4*i*k */
    seq_t diff = ii * ii - kk * kk;   /* i^2 - k^2 */
    seq_t result = prod * diff;       /* final term */

    return result;
}
