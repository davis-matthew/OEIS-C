/*
 * A073912  Smallest number m such that m and the product of digits of m are
 *          both divisible by 8n, or 0 if no such number exists.
 * https://oeis.org/A073912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m such that m and product of digits of m are
 *            both divisible by 8 n, or 0 if no such number exists
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the a(n) value for the OEIS sequence A073912.  */
seq_t a(idx_t n)
{
    /* If n is divisible by 5, or has any prime factor > 9,
       the answer is 0. */
    if (n % 5 == 0)
        return (seq_t)0;
    idx_t tmp = n;
    while (tmp % 2 == 0) tmp /= 2;
    while (tmp % 3 == 0) tmp /= 3;
    while (tmp % 7 == 0) tmp /= 7;
    if (tmp > 1)               /* remaining factor > 9 */
        return (seq_t)0;

    /* Target divisor: 8 * n. */
    seq_t T = (seq_t)8 * (seq_t)n;

    /* Search for a multiple of T that satisfies the digit‑product condition. */
    const idx_t MAX_K = 1000000;          /* generous upper bound */
    for (idx_t k = 1; k <= MAX_K; ++k) {
        seq_t m = T * (seq_t)k;

        /* Compute product of digits modulo T.  Skip numbers containing 0. */
        seq_t prod = (seq_t)1;
        seq_t x = m;
        bool hasZero = false;
        while (x > 0) {
            unsigned d = (unsigned)(x % (seq_t)10);
            if (d == 0) { hasZero = true; break; }
            prod = (prod * (seq_t)d) % T;
            x /= (seq_t)10;
        }
        if (hasZero) continue;               /* m contains a zero digit */

        if (prod == (seq_t)0) /* divisible by T */
            return m;
    }

    /* In theory this point should never be reached for admissible n. */
    return (seq_t)0;
}
