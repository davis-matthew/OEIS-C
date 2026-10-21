/*
 * A093732  Number of connected relations.
 * https://oeis.org/A093732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31^n - 5*16^n - 10*10^n + 20*9^n + 30*7^n - 60*6^n +
 *            24*5^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a power by repeated squaring.
   Works for the integer types that seq_t can denote (uint8_t ... uint128_t,
   or an arbitrary-precision class). */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* All calculations are done in seq_t to preserve precision. */
    seq_t term1 = seq_pow((seq_t)31, n);

    seq_t term2 = seq_pow((seq_t)16, n);
    term2 *= (seq_t)5;
    term2 = -term2;  /* subtract later */

    seq_t term3 = seq_pow((seq_t)10, n);
    term3 *= (seq_t)10;
    term3 = -term3;

    seq_t term4 = seq_pow((seq_t)9, n);
    term4 *= (seq_t)20;

    seq_t term5 = seq_pow((seq_t)7, n);
    term5 *= (seq_t)30;

    seq_t term6 = seq_pow((seq_t)6, n);
    term6 *= (seq_t)60;
    term6 = -term6;

    seq_t term7 = seq_pow((seq_t)5, n);
    term7 *= (seq_t)24;

    /* Combine all terms. */
    seq_t result = term1;
    result += term2;
    result += term3;
    result += term4;
    result += term5;
    result += term6;
    result += term7;

    return result;
}
