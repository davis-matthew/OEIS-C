/*
 * A018494  Divisors of 510.
 * https://oeis.org/A018494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th divisor of 510 (sorted ascending, a(1)=1)
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the n-th divisor of 510.  For n > 16 (the number of divisors)
   the function returns 0. */
seq_t a(idx_t n)
{
    /* Static storage for divisors and initialization flag */
    static seq_t divisors[16];
    static size_t divcnt = 0;
    static bool initialized = false;

    if (!initialized) {
        /* 510 factorisation (trial division, runtime to avoid hard‑coding primes) */
        uint64_t target = 510;
        uint64_t primes[10];
        int   exp   [10];
        size_t pcnt = 0;
        for (uint64_t p = 2; p * p <= target; ++p) {
            if (target % p == 0) {
                int e = 0;
                while (target % p == 0) {
                    target /= p;
                    ++e;
                }
                primes[pcnt] = p;
                exp   [pcnt] = e;
                ++pcnt;
            }
        }
        if (target > 1) {          /* remaining prime factor */
            primes[pcnt] = target;
            exp   [pcnt] = 1;
            ++pcnt;
        }

        /* Generate all divisors multiplicatively */
        size_t idx = 0;
        divisors[idx++] = 1;       /* start with divisor 1 */
        for (size_t pi = 0; pi < pcnt; ++pi) {
            size_t start = idx;    /* number of divisors before adding this prime */
            for (size_t i = 0; i < start; ++i) {
                seq_t val = divisors[i];
                for (int e = 0; e < exp[pi]; ++e) {
                    val *= (seq_t)primes[pi];
                    divisors[idx++] = val;
                }
            }
        }
        divcnt = idx;              /* should be 16 for 510 */

        /* Simple insertion sort (array is tiny) */
        for (size_t i = 1; i < divcnt; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        initialized = true;
    }

    if (n == 0)        /* safety, though offset is 1 */
        return (seq_t)0;
    if ((size_t)(n - 1) < divcnt)
        return divisors[n - 1];
    return (seq_t)0;   /* undefined for n > 16 */
}
