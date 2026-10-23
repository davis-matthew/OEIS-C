/*
 * A178526  Triangle read by rows: T(n,k) is the number of nodes of cost k in
 *          the Fibonacci tree of order n.
 * https://oeis.org/A178526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(k+1) if k<r, else F(r-1) Here r is the unique row index
 *            with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n – r*(r+1)/2.
 *            Special case r==0 yields a(n)=1. F(m) is the m‑th Fibonacci
 *            number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* iterative Fibonacci: returns F(n) */
static seq_t fib(idx_t n)
{
    seq_t a = 0;
    seq_t b = 1;
    while (n--) {
        seq_t t = a + b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    // Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
    idx_t low  = 0;
    idx_t high = n;
    idx_t r    = 0;
    while (low <= high) {
        idx_t mid    = low + (high - low) / 2;
        idx_t offset = mid * (mid + 1) / 2;
        if (offset <= n) {
            r   = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;      /* avoid underflow */
            high = mid - 1;
        }
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k      = n - offset;          /* column index within row r */

    if (r == 0) {
        return (seq_t)1;                /* row 0 has a single 1 */
    }
    if (k < r) {
        return fib(k + 1);              /* F(k+1)  for k < r */
    }                                 /* else k == r */
    return fib(r - 1);                 /* F(r-1)  for k == r */
}
