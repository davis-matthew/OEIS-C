/*
 * A074675  Three-digit distinct-digit primes.
 * https://oeis.org/A074675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth three‑digit prime that has all distinct decimal
 *            digits
 * Range:     u8 n <= 17, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   isPrimeInt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence is finite: there are 97 such primes, so a(n)=0 for n>97. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeInt.h"

/* Returns the nth term (1‑based) of A074675. */
seq_t a(idx_t n)
{
    static seq_t primes[97];
    static bool built = false;

    if (!built) {
        built = true;
        int count = 0;
        for (int num = 100; num <= 999; ++num) {
            if (!isPrimeInt(num)) continue;

            int d1 = num % 10;
            int d2 = (num / 10) % 10;
            int d3 = num / 100;
            if (d1 == d2 || d1 == d3 || d2 == d3) continue;

            primes[count++] = (seq_t)num;
        }
        /* After the loop count should be 97; if not, the array will simply be short. */
    }

    if (n >= 1 && n <= 97)
        return primes[n - 1];
    return (seq_t)0;
}
