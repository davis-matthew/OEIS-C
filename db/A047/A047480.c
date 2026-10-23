/*
 * A047480  Numbers that are congruent to {2, 5, 7} mod 8.
 * https://oeis.org/A047480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + {2,5,7}[ (n-1)%3 ]
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0 to
       satisfy the implicit expectation that the function is never called
       with an invalid index. */
    if (n == 0)
        return 0;

    /* Compute q = floor((n-1)/3) and r = (n-1) mod 3. */
    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;

    /* Base value 8*q is produced in seq_t to avoid overflow when q is
       large but still fits in the target integer type. */
    seq_t base = (seq_t)q * 8;

    /* Add the offset according to the residue. */
    switch (r) {
        case 0: return base + 2;   /* 8*q + 2 */
        case 1: return base + 5;   /* 8*q + 5 */
        case 2: return base + 7;   /* 8*q + 7 */
    }

    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
