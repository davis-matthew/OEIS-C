/*
 * A070953  Order of the group GU(n,2), the general unitary n X n matrices
 *          over the finite field GF(4).
 * https://oeis.org/A070953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * 2^(n(n-1)/2) * ∏_{i=1..(n-1)} (2^(i+1) - (-1)^(i+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    if (n == 1) return (seq_t)3;          /* a(1) = 3 */

    seq_t prod = (seq_t)1;
    seq_t pow  = (seq_t)2;                /* 2^1 */

    for (idx_t i = 1; i < n; ++i) {
        pow *= (seq_t)2;                  /* pow = 2^(i+1) */

        seq_t term;
        if (i & 1) {                     /* i odd  → (-1)^(i+1)= 1 → term = pow - 1 */
            term = pow - (seq_t)1;
        } else {                         /* i even → (-1)^(i+1)=-1 → term = pow + 1 */
            term = pow + (seq_t)1;
        }
        prod *= term;
    }

    prod *= (seq_t)3;                     /* multiply by the leading 3 */

    idx_t exp = n * (n - 1) / 2;          /* exponent of the extra 2-power */
    prod *= pow2_exp(exp);

    return prod;
}
