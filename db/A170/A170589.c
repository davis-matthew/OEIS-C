/*
 * A170589  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 4 * 3^(n-1) for n >= 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 3^e using binary exponentiation. */
static inline seq_t pow3(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (e) {
        if (e & 1) result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */
    /* n >= 1: a(n) = 4 * 3^(n-1) */
    seq_t val = pow3(n - 1);
    return val * (seq_t)4;
}
