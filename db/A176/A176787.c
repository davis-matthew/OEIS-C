/*
 * A176787  a(n) = (0!-1!+2!-3!....(-1)^(n-1)*(n-1)!) mod n.
 * https://oeis.org/A176787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (0! - 1! + 2! - 3! + ... + (-1)^(n-1)*(n-1)!) mod n
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1.  It runs in O(n) time and uses only
 * integer arithmetic that fits within seq_t, the build‑specified
 * unsigned integer type. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* n >= 1 in practice */

    seq_t modn = (seq_t)n;                 /* mod value */
    seq_t sum  = 0;                        /* running sum modulo n */
    seq_t fact = 1 % modn;                 /* 0! modulo n, becomes 0 when n == 1 */

    for (idx_t k = 0; k < n; ++k) {
        /* Add or subtract k! according to the sign pattern */
        if ((k & 1) == 0) {                /* even k: + */
            sum = (sum + fact) % modn;
        } else {                           /* odd k: - */
            sum = (sum + modn - fact) % modn;
        }

        /* Prepare factorial for (k+1)! for the next iteration,
           if such a term exists. */
        if (k + 1 < n) {
            fact = (fact * (seq_t)(k + 1)) % modn;
        }
    }
    return sum;
}
