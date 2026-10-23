/*
 * A143130  Triangle read by rows, A000012 * (A000332 * 0^(n-k)) * A000012,
 *          0<=k<=n.
 * https://oeis.org/A143130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r+4,5) - C(j+3,5) where r = smallest integer with
 *            r(r+1)/2 ≥ n j = n - r(r-1)/2 (1‑based position in the r‑th row)
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is an unsigned integer type, seq_t is an unsigned
   integer type of arbitrary precision.  All arithmetic is done
   without overflow.                                                */

#include <stdint.h>

static inline seq_t binom5(seq_t x)
{
    /* binomial coefficient C(x,5).  For x < 5 the result is 0. */
    if (x < (seq_t)(5))
        return (seq_t)0;

    seq_t res = (seq_t)1;
    /* Apply the multiplicative formula:
       C(x,5) = prod_{k=1..5} (x-(5-k))/k  */
    for (idx_t k = 1; k <= 5; ++k)
        res = res * (x - (seq_t)(5 - k)) / (seq_t)k;

    return res;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* offset 1 as per OEIS   */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = (idx_t)1;
    while (1)
    {
        /* Use __int128 to avoid overflow when n is large. */
        __int128 tri = (__int128)r * (r + 1) / 2;
        if (tri >= n)
            break;
        ++r;
    }

    /* Number of terms before row r.  prev = (r-1)*r/2. */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)prev;              /* 1‑based column index */

    /* Compute the value using the closed form. */
    seq_t c1 = binom5((seq_t)(r + 4));
    seq_t c2 = binom5((seq_t)(j + 3));
    return c1 - c2;
}
