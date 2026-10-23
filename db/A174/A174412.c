/*
 * A174412  A product triangle sequence based on:a=3;f(n, a) = f(n - 1, a) +
 *          a*f(n - 2, a); c(n,a)=If[n == 0, 1, Product[f(i, a)*f(i + 1, a),
 *          {i, 1, n}]]; t(n,m,a)=If[Floor[n/2] >= m, c(n, a)/c(n - m, a),
 *          c(n, a)/c(m, a)].
 * https://oeis.org/A174412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 30, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A174412: product triangle sequence for a = 3
    a(n) is obtained by first converting the linear index n into
    row r and column m according to the lower–triangular order
    (row 0 has one entry, row 1 has two, …):
        r = floor( (sqrt(8 n + 1) - 1) / 2 )
        m = n - r(r+1)/2
    Then
        t(r,m) =  c(r) / c(r - m)   if floor(r/2) ≥ m
                =  c(r) / c(m)      otherwise
    where
        f(0)=0,  f(1)=1,  f(k)=f(k-1)+3*f(k-2)   (k≥2)
        c(0)=1
        c(k)=∏_{i=1}^{k} f(i)*f(i+1)
*/

#include <stdlib.h>

static seq_t *f_arr = NULL;       /* f[0 .. last_computed]          */
static idx_t  f_cap = 0;          /* current length of f_arr         */

/* ensure that f_arr contains values up to index `up_to` (inclusive) */
static void ensure_f(idx_t up_to)
{
    if (f_arr == NULL) {
        /* initially require indices 0 and 1 */
        f_arr = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!f_arr) exit(1);
        f_arr[0] = (seq_t)0;
        f_arr[1] = (seq_t)1;
        f_cap    = 2;              /* i.e. indices 0..1 are valid      */
    }

    if ((idx_t)(f_cap - 1) >= up_to) return;   /* already computed all needed */

    size_t new_cap = (size_t)up_to + 1;         /* need indices 0..up_to inclusive */
    f_arr = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
    if (!f_arr) exit(1);

    for (idx_t i = f_cap; i <= up_to; ++i) {
        if (i >= 2) {
            f_arr[i] = f_arr[i-1] + (seq_t)3 * f_arr[i-2];
        }
    }
    f_cap = new_cap;
}

/* find row r such that r(r+1)/2 <= k < (r+1)(r+2)/2 */
static idx_t find_row(idx_t k)
{
    idx_t lo = 0, hi = 1;
    /* expand hi until offset > k */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= k) {
        hi <<= 1;
        if (hi == 0) break;          /* overflow guard (should never hit) */
    }
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= k)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* compute the value t(r,m) as described, without forming c(r) */
static seq_t t_value(idx_t r, idx_t m)
{
    if (m == 0 || m == r) return (seq_t)1;

    idx_t start_j, end_j;
    if ((r >> 1) >= m) {                 /* floor(r/2) >= m  */
        start_j = r - m + 1;
        end_j   = r;
    } else {
        start_j = m + 1;
        end_j   = r;
    }

    if (start_j > end_j) return (seq_t)1;    /* safety */

    /* need f up to end_j + 1 */
    ensure_f(end_j + 1);

    seq_t res = (seq_t)1;
    for (idx_t j = start_j; j <= end_j; ++j) {
        /* multiply by f[j] and f[j+1] */
        res = (unsigned __int128)res * f_arr[j];
        res = (unsigned __int128)res * f_arr[j + 1];
    }
    return res;
}

/* Public function: compute the n-th term of A174412 */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t m = n - offset;
    return t_value(r, m);
}
