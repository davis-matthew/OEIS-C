/*
 * A069847  Number of distinct primes obtained by inserting a 6 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting * a '6'
 *            into the decimal representation of n at every possible *
 *            position (including at the front). The result counts each prime
 *            * only once, even if it can be produced in more than one way
 * Range:     u8 n <= 60, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Convert n into its decimal digits (least‑to‑most).  We keep the
     * digits in order from most significant to least by reversing
     * the temporary buffer at the end.  n is guaranteed non‑negative. */
    char temp[32];
    int len = 0;
    idx_t t = n;
    if (t == 0) {
        temp[len++] = '0';
    } else {
        while (t > 0) {
            temp[len++] = '0' + (t % 10);
            t /= 10;
        }
        /* reverse to get most significant first */
        for (int i = 0; i < len / 2; ++i) {
            char c = temp[i];
            temp[i] = temp[len - 1 - i];
            temp[len - 1 - i] = c;
        }
    }

    /* Horner’s algorithm to compute the value when inserting a
     * '6' at position pos (0 <= pos <= len).  An array of distinct
     * primes found so far is kept; its size never exceeds len+1. */
    seq_t primes[20];
    int prime_count = 0;

    for (int pos = 0; pos <= len; ++pos) {
        idx_t val = 0;
        /* copy digits before the insertion point */
        for (int i = 0; i < pos; ++i)
            val = val * 10 + (temp[i] - '0');
        /* insert the digit 6 */
        val = val * 10 + 6;
        /* copy the remaining digits */
        for (int i = pos; i < len; ++i)
            val = val * 10 + (temp[i] - '0');

        /* test primality */
        if (!is_prime(val))
            continue;

        /* check whether we already counted this prime */
        bool seen = false;
        for (int j = 0; j < prime_count; ++j)
            if (primes[j] == val) { seen = true; break; }
        if (!seen) {
            primes[prime_count++] = val;
        }
    }

    return (seq_t)prime_count;
}
