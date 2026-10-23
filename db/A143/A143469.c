/*
 * A143469  Triangle read by rows, A000012 * A143315 * A128407, 1<=k<=1.
 * https://oeis.org/A143469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = (floor(r / c))² · μ(c) * where r is the row number such
 *            that r(r + 1)/2 ≥ n, * c is the column index in that row: c = n
 *            – (r – 1)·r/2. * μ is the Möbius function
 * Range:     i8 n <= 66, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*--- Trivial Möbius function ------------------------------------------------*/
static seq_t mu_value(idx_t x)
{
    if (x == 1) return (seq_t)1;
    idx_t n = x;
    int primes = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
            if (exp > 1) return (seq_t)0;           /* square factor */
        }
        if (exp == 1) primes++;
    }
    if (n > 1) primes++;                             /* remaining prime > sqrt(x) */
    return (primes & 1) ? (seq_t)(-1) : (seq_t)1;
}

/*--- Find row number -------------------------------------------------------*/
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* stretch hi until the triangular number exceeds n */
    while (((unsigned __int128)hi * (hi + 1) / 2) < n) hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/*--- Main sequence ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    idx_t r = find_row(n);                /* row number */
    idx_t tri_prev = (r - 1) * r / 2;     /* T(r‑1) */
    idx_t c = n - tri_prev;               /* column (1‑based) */

    idx_t div = r / c;                    /* floor(r / c) */
    seq_t sq = (seq_t)div * (seq_t)div;   /* (floor(r / c))² */
    seq_t mu = mu_value(c);               /* Möbius value */

    return sq * mu;
}
