/*
 * A122191  Triangle read by rows: T(n,m) = number of positive divisors of m
 *          that are coprime to n.
 * https://oeis.org/A122191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,m) where T(r,m) is the number of divisors d of m with
 *            gcd(d,r)=1, and r is the unique integer with r*(r-1)/2 < n ≤
 *            r*(r+1)/2. The column index m is m = n - r*(r-1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for the generic integer type seq_t.  */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the n-th term of sequence A122191.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /*  undefined for n < 1 – guard */

    /* Find the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {                   /* cum = r*(r+1)/2 after increment */
        r++;
        cum += r;
    }

    idx_t prev = cum - r;               /* sum of lengths of rows < r   */
    idx_t m = n - prev;                 /* column index (1‑based)      */

    /* Count divisors of m that are coprime to r. */
    seq_t ans = 0;
    for (idx_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            if (my_gcd(d, r) == 1) ++ans;
        }
    }

    return ans;
}
