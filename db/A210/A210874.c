/*
 * A210874  Triangular array U(n,k) of coefficients of polynomials defined in
 *          Comments.
 * https://oeis.org/A210874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = L(k) + (r‑k) * F(k+1) where r is the row such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2, and k = n - r(r‑1)/2. L(k) is the k‑th
 *            Lucas number (L(1)=1, L(2)=3, L(n)=L(n‑1)+L(n‑2)), F(m) is the
 *            Fibonacci number with F(1)=F(2)=1, F(m)=F(m‑1)+F(m‑2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row (r) by accumulating row lengths until the total
       exceeds or equals n. */
    idx_t row = 0, sum = 0;
    for (;; ++row) {
        sum += row;          /* sum = r(r+1)/2 after the increment */
        if (sum >= n)
            break;
    }
    idx_t r = row;                    /* current row number */
    idx_t prev_sum = sum - row;       /* sum of all previous rows */
    idx_t k = n - prev_sum;           /* column number, 1 ≤ k ≤ r */

    /* Compute Lucas number L(k). */
    seq_t lucas;
    if (k == 1) {
        lucas = 1;
    } else if (k == 2) {
        lucas = 3;
    } else {
        seq_t l1 = 1, l2 = 3;
        for (idx_t i = 3; i <= k; ++i) {
            seq_t l3 = l1 + l2;
            l1 = l2;
            l2 = l3;
        }
        lucas = l2;
    }

    /* Compute Fibonacci number F(k+1). */
    seq_t fib;
    if (k == 1) {
        /* F(2) = 1 */
        fib = 1;
    } else {
        /* Start from F(1)=1, F(2)=1 */
        seq_t f1 = 1, f2 = 1;
        for (idx_t i = 3; i <= k + 1; ++i) {
            seq_t f3 = f1 + f2;
            f1 = f2;
            f2 = f3;
        }
        fib = f2;
    }

    /* a(n) = L(k) + (r - k) * F(k+1) */
    seq_t diff = (seq_t)(r - k);
    return lucas + diff * fib;
}
