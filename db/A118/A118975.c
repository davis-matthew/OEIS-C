/*
 * A118975  Triangular array: row n consists of M times n-th-order Farey
 *          fractions, where M is the LCM of their denominators.
 * https://oeis.org/A118975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   farey_row_value, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A118975.
    The sequence is the flattened triangular array where row n
    consists of M · fractions of the n‑th Farey sequence (excluding 0),
    with M = lcm(1,2,…,n).
    This implementation evaluates the required term directly, using
    only integer arithmetic. */
#include "../utils/gcd.h"
#include "../utils/farey_row_value.h"

/* Trial‑division φ(r) for small arguments (r ≤ 1000). */
static idx_t phi_uint64(idx_t r)
{
    idx_t result = r;
    idx_t tmp = r;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* Least common multiple of 1..r, using the provided gcd(). */
static seq_t lcm_upto(idx_t r)
{
    seq_t l = 1;
    for (idx_t k = 2; k <= r; ++k)
    {
        seq_t g = gcd(l, (seq_t)k);
        l = l / g * (seq_t)k;
    }
    return l;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th term. */
    idx_t cumulativeTerms = 0;   /* terms up to previous row */
    idx_t cumulativePhi   = 0;   /* Σ φ(d) up to current row */
    idx_t r = 0;

    while (cumulativeTerms < n)
    {
        ++r;
        cumulativePhi += phi_uint64(r);          /* row length */
        cumulativeTerms += cumulativePhi;        /* cumulative terms */
    }

    /* Position within the found row (1‑based). */
    idx_t startIndex = cumulativeTerms - cumulativePhi + 1;
    idx_t offset     = n - startIndex + 1;       /* 1‑based offset in row r */

    /* Compute M = lcm(1..r). */
    seq_t M = lcm_upto(r);

    /* Return the requested value from row r. */
    return farey_row_value(r, offset, M);
}
