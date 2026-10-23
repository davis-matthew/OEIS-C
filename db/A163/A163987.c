/*
 * A163987  First differences of A160119.
 * https://oeis.org/A163987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   * a(1) = 1 * a(2) = 26 * For n ≥ 3: * if n is odd: a(n) = 8 *
 *            7^(popcount((n-1)/2) - 1) * if n is even: a(n) = 200 *
 *            7^(popcount(n/2 - 1) - 1) * Here popcount(x) is the number of
 *            1‑bits in the binary representation of x
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 26;

    idx_t k;
    int is_odd = n & 1LL;

    if (is_odd) {
        k = (n - 1) / 2;
    } else {
        k = n / 2 - 1;
    }

    /* compute popcount of k */
    unsigned long long uc = (unsigned long long)k;
    int popc = 0;
    while (uc) {
        uc &= uc - 1;
        ++popc;
    }

    /* exponent for 7: popc - 1 (k ≥ 1 guarantees popc ≥ 1) */
    int exp = popc - 1;

    seq_t pow7 = 1;
    for (int i = 0; i < exp; ++i)
        pow7 *= 7;

    if (is_odd) {
        return (seq_t)8 * pow7;
    } else {
        return (seq_t)200 * pow7;
    }
}
