/*
 * A166647  Totally multiplicative sequence with a(p) = 7*(p+1) for prime p.
 * https://oeis.org/A166647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (7*(p+1))^e (Totally multiplicative sequence
 *            with a(p) = 7*(p+1) for prime p)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t mul_pow(seq_t base, int exp) {
    seq_t r = 1;
    while (exp > 0) {
        r *= base;
        exp--;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* behaviour below offset not important */

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    uint64_t p;
    for (p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            seq_t base = (seq_t)7 * ((seq_t)p + 1);
            result *= mul_pow(base, e);
        }
    }

    if (tmp > 1) {                  /* remaining prime factor */
        seq_t base = (seq_t)7 * ((seq_t)tmp + 1);
        result *= mul_pow(base, 1);
    }

    return result;
}
