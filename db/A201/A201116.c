/*
 * A201116  Primes of the form 3n^3+7.
 * https://oeis.org/A201116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 3k³+7, with k starting at 0 *
 *            (so a(1) = 7, a(2) = 31, a(3) = 199, …)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* Find the nth prime of the form 3k^3+7 (k = 0,1,2,…). */
    idx_t count = 0;          /* number of primes found so far */
    idx_t k = 0;              /* current k */
    while (true) {
        /* Compute 3*k^3 + 7 in type seq_t */
        seq_t kk = (seq_t)k;
        seq_t val = (seq_t)3 * kk * kk * kk + (seq_t)7;
        if (is_prime_seq(val)) {
            ++count;
            if (count == n)
                return val; /* nth prime found */
        }
        ++k;
    }
}
