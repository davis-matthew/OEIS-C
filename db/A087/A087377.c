/*
 * A087377  Triangle read by rows: n-th row contains n smallest numbers k
 *          (say) such that nk+1 is a prime.
 * https://oeis.org/A087377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the k‑th smallest integer such that (n*k+1) is prime,
 *            the sequence obtained by listing these k’s row by row for n =
 *            1,2,3,... (row n has n terms)
 * Range:     u8 n <= 698, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safe guard */

    /* Find row r such that r*(r+1)/2 ≥ n  (row indices start at 1) */
    idx_t lo = 1, hi = n, r = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Position of n within its row (0‑based) */
    idx_t start = (r - 1) * r / 2 + 1;     /* first index of row r */
    idx_t col   = n - start;               /* 0‑based column index */

    /* Scan k = 1,2,3,… until we find the (col)‑th prime of the form r*k+1 */
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        idx_t val = r * k + 1;
        if (isPrime(val)) {
            if (count == col) {
                return (seq_t)k;
            }
            ++count;
        }
        ++k;
    }
}
