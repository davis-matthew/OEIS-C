/*
 * A130461  Triangle, antidiagonals of an array generated from A130460.
 * https://oeis.org/A130461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the entry a(r,c) of an infinite array defined by
 *
 *      a(r,c) = c! / (c-r)!         if c ≥ r
 *      a(r,c) = c!                  if c < r
 *
 * The indices (r,c) are obtained from n by writing n as
 *    n = d(d+1)/2 + k          (0 ≤ k ≤ d)
 * and taking
 *      r = d - k
 *      c = k
 *
 * This reproduces the OEIS sequence A130461.
 */
seq_t a(idx_t n)
{
    /* determine the diagonal index d such that t_d ≤ n < t_{d+1}  */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* triangular number t_d */
    unsigned __int128 t_d = (unsigned __int128)d * (d + 1) / 2;

    /* offset on the diagonal k = n - t_d */
    idx_t k = (idx_t)(n - t_d);

    /* coordinates of the element */
    idx_t r = d - k;  /* row  */
    idx_t c = k;      /* column */

    seq_t result = 1;

    if (c >= r) {
        /* compute product (c - r + 1) * ... * c = c! / (c - r)! */
        idx_t start = c - r + 1;
        for (idx_t t = start; t <= c; ++t)
            result *= (seq_t)t;
    } else {
        /* compute c!  */
        for (idx_t t = 2; t <= c; ++t)
            result *= (seq_t)t;
    }

    return result;
}
