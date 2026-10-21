/*
 * A214119  Irregular array T(n,k) of the numbers of non-extendable (complete)
 *          non-self-adjacent simple paths ending at each of a minimal subset
 *          of nodes within a square lattice bounded by rectangles with nodal
 *          dimensions n and 2, n >= 2.
 * https://oeis.org/A214119
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened irregular array of T(n,k) where T(n,1) =
 *            Fibonacci(n+3) - 3 (for n ≥ 2) and T(n,k) = 0 for k ≥ 2. Each
 *            row n has width floor((n+1)/2). The sequence starts at a(2) =
 *            T(2,1)
 * Range:     u8 n <= 30, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided Fibonacci routine */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for indices < 2 */

    /* j: 0‑based position in the flattened sequence */
    idx_t j   = n - 2;
    idx_t sum = 0;                /* number of entries before current row */

    for (idx_t r = 2; ; ++r)     /* rows start at n = 2 */
    {
        /* width of row r: floor((r+1)/2) */
        idx_t w = (r + 1) >> 1;   /* integer division by 2 */

        if (sum + w > j)          /* the desired term is here */
        {
            idx_t col = j - sum + 1;   /* 1‑based column in the row */

            if (col == 1)          /* only the first column is non‑zero */
            {
                seq_t v = fib(r + 3);   /* Fibonacci(r+3) */
                v -= (seq_t)3;          /* subtract 3 */
                return v;
            }
            else                     /* other columns are zero */
            {
                return (seq_t)0;
            }
        }
        sum += w;                /* move to next row */
    }
}
