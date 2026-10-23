/*
 * A166652  Totally multiplicative sequence with a(p) = 3p-1 for prime p.
 * https://oeis.org/A166652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (3p‑1)^e (multiplicative, a(p)=3p‑1)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t t = (uint64_t)n;

    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int exp = 0;
            seq_t base = (seq_t)(3 * p - 1);
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            for (int i = 0; i < exp; ++i)
                result *= base;
        }
    }

    /* If t>1, it is a remaining prime factor with exponent 1. */
    if (t > 1) {
        seq_t base = (seq_t)(3 * t - 1);
        result *= base;
    }

    return result;
}
