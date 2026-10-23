/*
 * A143269  Triangle read by rows, A127648 * A000012 * A130207, 1<=k<=n.
 * https://oeis.org/A143269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r * φ(k) where the triangle is read row‑by‑row, with rows
 *            r = 1,2,… For a given index n (1‑based), r = ⌈(√(8 n + 1) –
 *            1)/2⌉, k = n – r(r – 1)/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute Euler’s totient φ(m) for a 64‑bit integer m.
   The calculation is performed in 64‑bit arithmetic and the result is
   cast to seq_t at the end.  This is safe because the values of m that
   appear in the sequence are small (≤ ≈ √(2 n) ≤ 200 for n ≤ 10 000). */
static seq_t phi_u64(uint64_t m)
{
    unsigned long long res = m;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            res = res / p * (p - 1);
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        res = res / tmp * (tmp - 1);
    }
    return (seq_t)res;
}

/* Find the row number r such that the total number of terms up to row r
   is at least n (i.e. r(r+1)/2 ≥ n).  Binary search on 64‑bit integers. */
static idx_t
row_of(idx_t n)
{
    idx_t lo = 1, hi = 2 * n;            /* upper bound satisfies inequality */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

/* Return the n-th term of OEIS sequence A143269 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* no value for index 0 */

    idx_t r = row_of(n);                     /* row number */
    idx_t prev = (r - 1) * r / 2;            /* terms before this row */
    idx_t k = n - prev;                      /* column index (1‑based) */

    seq_t phi_k = phi_u64((uint64_t)k);
    return (seq_t)r * phi_k;
}
