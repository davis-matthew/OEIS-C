/*
 * A237444  Triangle read by rows, T(n,k) is difference of column sum and row
 *          sum of natural numbers filled in n x n square.
 * https://oeis.org/A237444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = n/2 * (n^2 - 2*n*k + 2*k - 1), a(n) = T(r,c) where r is
 *            the row index: r = smallest integer such that r(r+1)/2 ≥ n, c =
 *            n – (r−1)r/2
 * Range:     i8 n <= 28, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, but safe for completeness */
        return 0;

    /* Find the row r that contains position n. */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;                     /* double until big enough */
        if (high == 0) {                /* overflow: cap at maximum */
            high = (idx_t)-1;
            break;
        }
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position inside the row. */
    idx_t prev = (r - 1) * r / 2;   /* elements before row r */
    idx_t c = n - prev;             /* 1‑based column index */

    /* Compute T(r,c) using extended integer arithmetic to avoid overflow. */
    __int128 rr = (__int128)r;
    __int128 cc = (__int128)c;
    __int128 bracket = rr * rr - 2 * rr * cc + 2 * cc - 1;   /* r^2 - 2rc + 2c - 1 */
    __int128 prod     = rr * bracket;                       /* r * bracket */
    seq_t result = (seq_t)(prod / 2);                       /* n/2 * bracket */

    return result;
}
