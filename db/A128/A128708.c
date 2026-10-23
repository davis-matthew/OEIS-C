/*
 * A128708  Let j(n) be the Jacobsthal function, A048669. Then a(n) is the
 *          number of times that the gap j(n) appears between consecutive
 *          numbers <= n+1 and coprime to n.
 * https://oeis.org/A128708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computes the number of occurrences of the maximum gap between
   successive integers ≤ n+1 that are coprime to n.
   The maximum gap equals the Jacobsthal function j(n). */

#include <stdint.h>

/* Compute gcd for idx_t (unsigned 64‑bit) */
static inline idx_t ugcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Guard for n < 1 – undefined behaviour, return 0 */
    if (n == 0)
        return 0;

    idx_t last = 0;
    idx_t first_seen = 0;
    seq_t max_gap = 0;
    seq_t count = 0;

    /* Iterate over all numbers 1 … n+1 */
    for (idx_t m = 1; m <= n + 1; ++m) {
        if (ugcd(m, n) == 1) {
            if (first_seen == 0) {
                /* First coprime number found */
                first_seen = m;
            } else {
                /* Gap between consecutive coprime numbers */
                idx_t g = m - last;
                seq_t diff = (seq_t)g;
                if (diff > max_gap) {
                    max_gap = diff;
                    count = 1;
                } else if (diff == max_gap) {
                    ++count;
                }
            }
            last = m;
        }
    }

    return count;
}
