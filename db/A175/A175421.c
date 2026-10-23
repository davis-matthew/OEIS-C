/*
 * A175421  Sequence of numbers after 2nd step of iteration defined in
 *          A175419.
 * https://oeis.org/A175421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = T(T(n)) * where T(m) is the left‑associated
 *            exponentiation of the decimal digits * of m, starting from the
 *            least‑significant digit. * Example: m = 6561 → ((1^6)^5)^6 = 1
 * Range:     u8 n <= 26, u16 n <= 36, u32 n <= 38, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses an external big_pow function for the
 * power operation. */

#include "../utils/big_pow.h"

static seq_t digit_power(seq_t n)
{
    if (n == (seq_t)0)
        return (seq_t)0;          /* defined as 0 for n = 0 */

    seq_t result = n % (seq_t)10;  /* least‑significant digit */
    n /= (seq_t)10;

    while (n > (seq_t)0) {
        seq_t d = n % (seq_t)10;   /* next digit (toward more significant) */
        result = big_pow(result, d); /* left‑assoc exponentiation */
        n /= (seq_t)10;
    }

    return result;
}

seq_t a(idx_t n)
{
    seq_t first = digit_power((seq_t)n);
    return digit_power(first);
}
