/*
 * A141529  A cyclotomic Binet solution for the first 11 primes: solution of a
 *          set of linear equations with cyclotomic polynomial roots; roots:
 *          r[i]->(x^11-1): a(n)=Sum[r(i)^n*c(i),{i,1,11}].
 * https://oeis.org/A141529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) is the sequence of the first 11 primes in decreasing order
 *            repeated periodically with period 11. Thus a(n) = p[11 – (n-1
 *            mod 11)], where p = {2,3,5,7,11,13,17,19,23,29,31}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t primes_desc[11];
    static bool initialized = false;
    if (!initialized) {
        seq_t asc[11];
        idx_t count = 0;
        idx_t candidate = 2;
        while (count < 11) {
            if (is_prime_small((seq_t)candidate)) {
                asc[count++] = (seq_t)candidate;
            }
            candidate++;
        }
        for (idx_t i = 0; i < 11; ++i) {
            primes_desc[i] = asc[10 - i];
        }
        initialized = true;
    }
    idx_t idx = (n - 1) % 11;
    return primes_desc[idx];
}
