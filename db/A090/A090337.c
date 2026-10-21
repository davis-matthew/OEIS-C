/*
 * A090337  Let b(0) = 1, b(n) = b(n-1) + (-1)^(n-1)*b(n-1)/10; sequence gives
 *          numerator of b(n).
 * https://oeis.org/A090337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 99^(floor(n/2)) * (n%2 ? 11 : 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp using binary exponentiation.
   Works with any seq_t type that supports * and +/>, ==. */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t half = n >> 1;           /* floor(n/2) */
    seq_t val   = seq_pow((seq_t)99, half);
    if (n & 1)                     /* odd n */
        val *= (seq_t)11;
    return val;
}
