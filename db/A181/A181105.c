/*
 * A181105  Triangle T(n,k) read by rows. T(n,k)=abs(A181101)*n/k.
 * https://oeis.org/A181105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A181105: Triangle T(n,k)=spf(n) if k=n/spf(n), otherwise 0.
 *  The sequence is read by rows of the triangle.  For a given index n
 *  (starting at 1), compute the row r such that r(r+1)/2 >= n;
 *  let c = n - r(r-1)/2 be the column (1 <= c <= r).
 *  Then a(n) = spf(r) if c == r/spf(r), else 0.
 *  spf(1) is defined as 1.  The code uses only integer arithmetic.
 */

#include <stdbool.h>

static seq_t spf(idx_t n) {
    if (n <= 1)
        return (seq_t)1;
    if (n % 2 == 0)
        return (seq_t)2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0)
            return (seq_t)i;
    }
    return (seq_t)n;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* find row r: minimal r with r(r+1)/2 >= n */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* column index within the row (1‑based) */
    idx_t prev = (idx_t)(((unsigned __int128)r * (r - 1)) / 2);
    idx_t c = n - prev;

    seq_t sp = spf(r);
    idx_t k = r / (idx_t)sp;   /* column of the non‑zero entry */

    return (c == k) ? sp : (seq_t)0;
}
