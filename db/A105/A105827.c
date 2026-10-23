/*
 * A105827  a(n) = sigma(n) (mod 8).
 * https://oeis.org/A105827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) (mod 8)
 * Range:     u8 n <= 9568, u16 n <= 9568, u32 n <= 9568, u64 n <= 9568,
 *            u128 n <= 9568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0, return 0 */
    seq_t result = 1;
    idx_t x = n;

    /* factor 2 */
    if ((x & 1ULL) == 0) {
        int e = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++e;
        }
        seq_t term = (e == 1) ? 3    /* 1+2 */
                               : 7; /* 1+2+4 (+0 for higher powers) */
        result = (result * term) % 8;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            seq_t term = 1;
            seq_t pk   = 1;                     /* p^0 mod 8 */
            seq_t pmod = p & 7ULL;              /* p mod 8 */
            for (int i = 0; i < e; ++i) {
                pk   = (pk * pmod) % 8;
                term = (term + pk) % 8;
            }
            result = (result * term) % 8;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (x > 1) {
        seq_t term = (1 + (x & 7ULL)) % 8;     /* 1 + p mod 8 */
        result = (result * term) % 8;
    }

    return result;
}
