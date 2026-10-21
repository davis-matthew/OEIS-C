/*
 * A236991  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A236991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1944*a(n-4) for n ≥ 5; a(1) = 360, a(2) = 2376, a(3) =
 *            15552, a(4) = 104976
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the residue class of n modulo 4 */
    idx_t r = n % 4;
    /* Base term for this class */
    seq_t base;
    if (r == 1) {
        base = (seq_t)360;
    } else if (r == 2) {
        base = (seq_t)2376;
    } else if (r == 3) {
        base = (seq_t)15552;
    } else {          /* r == 0, i.e., n ≡ 0 (mod 4), n >= 4 */
        base = (seq_t)104976;
    }

    /* Compute the exponent of 1944 to multiply */
    idx_t exp;
    if (r == 0) {
        /* n = 4 + 4*k  (k ≥ 0) */
        exp = (n - 4) / 4;
    } else {
        /* n = r + 4*k (k ≥ 0) where r = 1,2,3 */
        exp = (n - r) / 4;
    }

    /* Multiply base by 1944^exp */
    seq_t result = base;
    const seq_t mult = (seq_t)1944;
    for (idx_t i = 0; i < exp; ++i) {
        result *= mult;
    }
    return result;
}
