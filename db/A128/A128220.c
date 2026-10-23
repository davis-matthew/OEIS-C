/*
 * A128220  Triangle, A127701 * A000012.
 * https://oeis.org/A128220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r if n is the last element of row r, otherwise a(n) = r +
 *            1, where r is the unique integer with r(r+1)/2 ≥ n. The triangle
 *            rows are: (r+1) repeated (r‑1) times followed by r
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: find the row r such that r(r+1)/2 >= n.
   Uses binary search on 64‑bit integers; the product is computed in 128 bits. */
static uint64_t
row_of_index(uint64_t n)
{
    uint64_t low = 0, high = n;          /* high is inclusive */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)                                 /* offset starts at 1 – undefined below */
        return 0;

    uint64_t r = row_of_index((uint64_t)n);      /* row number */

    /* linear index at which row r starts: 1 + sum_{i=1}^{r-1} i = r(r-1)/2 + 1 */
    unsigned __int128 start = (unsigned __int128)r * (r - 1) / 2 + 1;

    /* position within the row (0‑based) */
    uint64_t k = (uint64_t)n - (uint64_t)start;

    if (k == r - 1)                 /* last element of the row */
        return (seq_t)r;
    else                            /* all preceding elements are r+1 */
        return (seq_t)(r + 1);
}
