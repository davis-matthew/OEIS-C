/*
 * A072806  Primes of the form 6k+5 written in base 5.
 * https://oeis.org/A072806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th prime of the form 6k+5, expressed in base‑5 but
 *            written as a decimal integer whose digits are that base‑5
 *            representation
 * Range:     u8 n <= 10, u16 n <= 227, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the n‑th prime of the form 6k+5. */
    idx_t count = 0;
    idx_t candidate = 5;                /* first 6k+5 candidate */

    while (true) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                break;
        }
        candidate += 6;                 /* next 6k+5 */
    }

    /* Convert the prime to its base‑5 digits. */
    seq_t tmp = (seq_t)candidate;
    int digits[64];
    int dcnt = 0;
    while (tmp > (seq_t)0) {
        digits[dcnt++] = (int)(tmp % (seq_t)5);
        tmp /= (seq_t)5;
    }
    if (dcnt == 0)                       /* never happens for our primes */
        digits[dcnt++] = 0;

    /* Interpret those digits as a decimal number. */
    seq_t val = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        val = val * (seq_t)10 + (seq_t)digits[i];

    return val;
}
