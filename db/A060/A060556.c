/*
 * A060556  Bisection of triangle A060098: odd-indexed members of column
 *          sequences of A060098 (not counting leading zeros).
 * https://oeis.org/A060556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th term obtained by concatenating the rows of
 * the triangle A060098.  For row r and column m (0‑based)
 * the triangle entry is
 *    T(r,m) = Σ_{j=0}^{⌊(m+1)/2⌋} C(r - j + m, 2m) * C(m+1, 2j+1)
 * where C(a,b) = binomial(a,b).  The sequence a(n) is produced by
 * a(n) = T(r,m) where r = floor((√(8n+1)-1)/2) and
 * m = n - r(r+1)/2.
 *
 * This implementation computes r by a simple linear search (fast enough
 * for n up to at least 10 000) and evaluates the above sum using the
 * provided binomial routine.  All arithmetic that can overflow 64‑bit
 * integers is performed in unsigned __int128, which is supported by
 * GCC/Clang and is accepted by the C++17 compiler in the test harness.
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Compute the column index within that row. */
    idx_t before = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t m = n - before;

    /* Evaluate the triangle entry a(r,m) using the explicit formula. */
    seq_t result = 0;
    idx_t maxJ = (m + 1) / 2;
    for (idx_t j = 0; j <= maxJ; ++j)
    {
        seq_t t1 = binomial(r - j + m, 2 * m);
        seq_t t2 = binomial(m + 1, 2 * j + 1);
        result += t1 * t2;
    }
    return result;
}
