/*
 * A164928  Sum of the odd prime divisors of numbers whose odd prime divisors
 *          are all of the form 4k+3.
 * https://oeis.org/A164928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a164928:
   a(n) = sum of the distinct odd prime divisors of the n-th integer
   whose odd prime factors are all of the form 4k+3.  The sequence
   is indexed from 1; a(1) is the first term listed in the OEIS entry. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* no definition for n = 0 */

    idx_t found = 0;
    uint64_t num = 3;                      /* first candidate with odd primes */

    while (true) {
        uint64_t t = num;

        /* remove all factors of 2 – they are ignored */
        while ((t & 1ULL) == 0) t >>= 1;
        if (t <= 1) {                      /* pure power of two – skip */
            num++;
            continue;
        }

        bool ok = true;
        uint64_t sum = 0;
        uint64_t f = t;

        /* trial division for odd factors up to sqrt(f) */
        for (uint64_t p = 3; p * p <= f; p += 2) {
            if (f % p == 0) {
                if (p % 4 != 3) {          /* not of the form 4k+3 */
                    ok = false;
                    break;
                }
                sum += p;                   /* distinct prime, add once */
                while (f % p == 0) f /= p; /* remove all powers */
            }
        }

        if (!ok) {                          /* reject this candidate */
            num++;
            continue;
        }

        /* any remaining factor > 1 is itself a prime */
        if (f > 1) {
            if (f % 4 != 3) {
                num++;
                continue;
            }
            sum += f;
        }

        /* this number qualifies – record it */
        found++;
        if (found == n)                      /* the n-th term found */
            return (seq_t)sum;

        num++;                               /* next candidate */
    }
}
