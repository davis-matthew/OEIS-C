/*
 * A102498  Values of n for which the concatenation 3nn3 (from sequence
 *          A100896) are primes.
 * https://oeis.org/A102498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   count_digits, isprime_big, pow10, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer n such that 3nn3 (concatenated in base 10) is a prime number. */
/* This implements OEIS A102498. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_digits.h"
#include "../utils/pow10.h"
#include "../utils/isprime_big.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static idx_t cap    = 0;
    static idx_t size   = 0;
    static idx_t next   = 1;          /* next candidate integer n */

    while (size <= n) {
        seq_t nval = (seq_t)next;
        int d = count_digits(next);   /* number of decimal digits of n */

        seq_t part1 = pow10((idx_t)(2 * d + 1)) * (seq_t)3;
        seq_t part2 = pow10((idx_t)(d + 1)) * nval;
        seq_t part3 = nval * (seq_t)10;
        seq_t num   = part1 + part2 + part3 + (seq_t)3;

        if (isprime_big(num))
            push_seq(&values, &cap, &size, nval);

        ++next;
    }
    return values[n];
}
