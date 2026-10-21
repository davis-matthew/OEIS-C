/*
 * A237368  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median plus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 196 * a(n-4) for n > 6 Initial terms: a(1) = 81, a(2) =
 *            288, a(3) = 1024, a(4) = 3840, a(5) = 14400, a(6) = 53760 The
 *            recurrence holds for all n > 6. The implementation computes
 *            values directly using this recurrence and repeated
 *            multiplication by 196
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small table of the first 6 values (index 1..6) */
    static const seq_t init[7] = {0, 81, 288, 1024, 3840, 14400, 53760};

    if (n <= 6)
        return init[n];

    /* determine the base index of the same residue mod 4 */
    seq_t base;
    idx_t  base_index;
    switch (n % 4) {
        case 1: base_index = 5; base = init[5]; break; /* n ≡ 1 (mod 4), n≥5 */
        case 2: base_index = 6; base = init[6]; break; /* n ≡ 2 (mod 4), n≥6 */
        case 3: base_index = 3; base = init[3]; break; /* n ≡ 3 (mod 4), n≥3 */
        case 0: base_index = 4; base = init[4]; break; /* n ≡ 0 (mod 4), n≥4 */
        default: /* unreachable */ return 0;
    }

    /* exponent count for powers of 196 */
    idx_t exp = (n - base_index) / 4;
    seq_t result = base;
    for (idx_t i = 0; i < exp; ++i)
        result = result * (seq_t)196;   /* repeated multiplication by 196 */

    return result;
}
