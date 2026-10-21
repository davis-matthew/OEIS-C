/*
 * A096555  Consecutive internal states of the linear congruential
 *          pseudo-random number generator RANDU that was used in the IBM
 *          Scientific Subroutine Library for IBM System/360 computers in the
 *          1970's.
 * https://oeis.org/A096555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 65539 * a(n-1) mod 2^31 with a(1) = 1. This recurrence
 *            can be solved by exponentiation: a(n) = 65539^(n-1) mod 2^31
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _RANDU_MULT = (seq_t)65539;
static const seq_t _RANDU_MASK = ((seq_t)1 << 31) - 1;   /* lower 31 bits */

static seq_t _modpow_2pow31(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1)  res = (res * base) & _RANDU_MASK;
        base = (base * base) & _RANDU_MASK;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* offset is 1; n<1 is undefined */
    /* a(1) = 1 = 65539^0 mod 2^31 */
    return _modpow_2pow31(_RANDU_MULT, n - 1);
}
