/*
 * A025906  Expansion of 1/((1-x^6)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+9)*(n+18)/1320 + (n+10)*(-1)^n/120 +
 *            ((n^2+5*n+9) mod 11)/11)
 * Range:     u8 n <= 1, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;

    /* first term: (n+9)*(n+18) */
    seq_t term1 = (n_seq + 9) * (n_seq + 18);

    /* second term: 11*(n+10) * ((-1)^n) */
    seq_t term2 = (n_seq + 10) * 11;

    /* third term: ((n^2 + 5n + 9) mod 11) * 120
       Compute the remainder modulo 11 using the fact that only the
       remainder of n modulo 11 matters to avoid overflow. */
    seq_t n_mod = n_seq % (seq_t)11;
    seq_t rem = (n_mod * n_mod + 5 * n_mod + 9) % (seq_t)11;
    seq_t term3 = rem * 120;

    /* Combine the terms with the appropriate sign for term2. */
    seq_t sum = term1 + term3;
    if ((n & 1ULL) == 0) {
        sum += term2;        /* even n: add term2 */
    } else {
        sum -= term2;        /* odd n : subtract term2 */
    }

    /* Final result */
    return sum / (seq_t)1320;
}
