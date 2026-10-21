/*
 * A061704  Number of cubes dividing n.
 * https://oeis.org/A061704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (⌊e/3⌋ + 1) (Number of cube divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 */
    idx_t exp = 0;
    while ((temp & 1ULL) == 0) {
        exp++;
        temp >>= 1;
    }
    if (exp) result *= (seq_t)(exp/3 + 1);

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            result *= (seq_t)(exp/3 + 1);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        /* exponent is 1 => floor(1/3)+1 = 1 */
        result *= (seq_t)1;
    }

    return result;
}
