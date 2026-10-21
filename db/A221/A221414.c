/*
 * A221414  Number of nX3 arrays of occupancy after each element stays put or
 *          moves to some horizontal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; a(2k) = 10 * 15^(k-1) (k >= 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of n×3 occupancy arrays with zero or two occupants
   after each element either stays put or moves to a horizontal or
   antidiagonal neighbour.  The sequence satisfies a(1)=0, a(2)=10 and
   a(n)=15*a(n-2) for n≥3. */

/* The implementation uses fast exponentiation for 15^(n/2-1).  The
   result is computed in type seq_t, which may be an unsigned integer
   of any width or an arbitrary-precision type.  No auxiliary memory
   is allocated.  The function is safe for any non‑negative value of
   n≥1. */

seq_t a(idx_t n)
{
    /* Handle the first two values explicitly. */
    if (n == (idx_t)1)
        return (seq_t)0;
    if (n == (idx_t)2)
        return (seq_t)10;

    /* Odd indices produce zero. */
    if (n & (idx_t)1)
        return (seq_t)0;

    /* Even n: compute 10 * 15^(n/2 - 1). */
    idx_t e = n / (idx_t)2 - (idx_t)1;   /* exponent for 15 */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)15;

    while (e)
    {
        if (e & (idx_t)1)
            result *= base;
        base *= base;
        e >>= (idx_t)1;
    }

    result *= (seq_t)10;
    return result;
}
