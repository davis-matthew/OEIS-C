/*
 * A167323  Totally multiplicative sequence with a(p) = 4*(p+3) for prime p.
 * https://oeis.org/A167323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p^e‖n} (4 * (p + 3))^e
 *          (product over prime powers in the factorisation of n)
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factor 2 */
    int e = 0;
    while ((m & 1ULL) == 0) {
        e++;
        m >>= 1;
    }
    if (e > 0) {
        seq_t base = (seq_t)4 * ((seq_t)2 + (seq_t)3);
        seq_t pow = (seq_t)1;
        for (int i = 0; i < e; i++) pow *= base;
        result *= pow;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) {
            e++;
            m /= p;
        }
        if (e > 0) {
            seq_t base = (seq_t)4 * ((seq_t)p + (seq_t)3);
            seq_t pow = (seq_t)1;
            for (int i = 0; i < e; i++) pow *= base;
            result *= pow;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t base = (seq_t)4 * ((seq_t)m + (seq_t)3);
        result *= base;
    }

    return result;
}
