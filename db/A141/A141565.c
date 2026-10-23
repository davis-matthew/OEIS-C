/*
 * A141565  Add 1 to all bases and exponents which are greater than 1 in the
 *          prime number decomposition of n.
 * https://oeis.org/A141565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n = 1, otherwise a(n) = ∏_{p^e‖n} (p+1)^{ e==1 ? 1 :
 *            e+1 }
 * Range:     u8 n <= 23, u16 n <= 575, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: exponentiation by squaring (seq_t can be any unsigned integer or big‑int) */
static seq_t pow_seq_t(seq_t base, uint64_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 1)                 /* special case, OEIS list starts with 2 */
        return (seq_t)2;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* n fits in uint64_t */

    for (uint64_t p = 2; p * p <= m; p += (p == 2) ? 1 : 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t base = (seq_t)(p + 1);
            seq_t powerExp = (exp == 1) ? base : pow_seq_t(base, exp + 1);
            result *= powerExp;
        }
    }

    /* Remaining prime factor if any */
    if (m > 1) {
        seq_t base = (seq_t)(m + 1);
        result *= base;           /* exponent is 1 */
    }

    return result;
}
