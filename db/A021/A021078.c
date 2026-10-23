/*
 * A021078  Decimal expansion of 1/74.
 * https://oeis.org/A021078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = (n == 0) ? 0 : ((2*n - 1) % 6)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Compute 2*n - 1 mod 6 without overflow by reducing n modulo 3 */
    switch (n % 3) {
        case 1: return 1; /* (2·1−1) mod 6 = 1 */
        case 2: return 3; /* (2·2−1) mod 6 = 3 */
        default: return 5; /* n % 3 == 0 gives (2·3−1) mod 6 = 5 */
    }
}
