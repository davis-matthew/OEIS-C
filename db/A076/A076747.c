/*
 * A076747  List giving pairs of primes of the form 10k+3 and 10k+13.
 * https://oeis.org/A076747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 584, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076747:  list of primes that appear in pairs of the form
 *  (10k+3, 10k+13).  Each valid pair contributes two successive terms:
 *  first the 10k+3 prime, then the 10k+13 prime.  Duplicates can occur
 *  when a prime is both a 10k+3 and a 10(k-1)+13.  The sequence
 *  starts with 3,13,13,23,43,53,...  (see OEIS entry for full list). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset < 1; return 0 as sentinel */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        idx_t p1 = 10 * k + 3;
        idx_t p2 = 10 * k + 13;

        if (is_prime(p1) && is_prime(p2))
        {
            ++count;
            if (count == n) return (seq_t)p1;

            ++count;
            if (count == n) return (seq_t)p2;
        }
    }
}
