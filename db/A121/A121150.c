/*
 * A121150  Minimal number of vertices in an n-polyomino.
 * https://oeis.org/A121150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n + floor(sqrt(4n-3)) + 2 for n ≥ 1
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root of a 64‑bit unsigned value.
   The algorithm uses only integer arithmetic and is safe for all
   values that fit in idx_t. */
static idx_t uint64_isqrt(idx_t x)
{
    if (x == 0) return 0;
    idx_t r = x;
    while (r > x / r) r--;          /* r*r > x ⇒ decrement */
    return r;
}

/* Public entry point as required by the build system */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0
       (this case never occurs in the tests, but it makes the
       function total). */
    if (n == 0) return 0;

    /* Compute 4n – 3 in the wider idx_t type to avoid overflow for
       the supported range of n. */
    idx_t tmp = 4 * n - 3;

    /* Compute floor(sqrt(4n-3)) using the helper defined above. */
    idx_t root = uint64_isqrt(tmp);

    /* Build the result in seq_t. */
    return (seq_t)n + (seq_t)root + 2;
}
