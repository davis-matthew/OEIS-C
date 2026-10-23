/*
 * A039948  A triangle related to A000045 (Fibonacci numbers).
 * https://oeis.org/A039948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * The triangle entries are defined by * T(n, m) = n! *
 *            Fibonacci(n-m+1) / m!, 0 ≤ m ≤ n. * The sequence a(n) is the
 *            row‑major flattening of this triangle: * a(n) = T(r, m) where r
 *            = floor((sqrt(8n+1)-1)/2) and * m = n - r*(r+1)/2. *
 *            Fibonacci(0)=0, Fibonacci(1)=1
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 170,
 *            u128 n <= 464, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        unsigned __int128 prod = (unsigned __int128)r * (r + 1);
        unsigned __int128 cnt  = prod / 2;
        if (cnt > n) break;
        r++;
    }
    r--; /* last r that satisfies r*(r+1)/2 ≤ n */

    /* Number of terms before this row */
    unsigned __int128 prod_prev = (unsigned __int128)r * (r + 1);
    idx_t terms_before = (idx_t)(prod_prev / 2);

    /* Column within the row */
    idx_t m = n - terms_before;

    /* Factorial ratio n! / m! = ∏_{i=m+1}^{n} i */
    seq_t ratio = 1;
    for (idx_t i = m + 1; i <= r; ++i)
        ratio *= (seq_t)i;

    /* Fibonacci index */
    idx_t fib_index = r - m + 1;
    seq_t fib_val = fib(fib_index);

    return ratio * fib_val;
}
