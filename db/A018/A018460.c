/*
 * A018460  Divisors of 444.
 * https://oeis.org/A018460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 444 in increasing order */
#include <stdint.h>
#include <stddef.h>

/* external declaration (in prelude) */
seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Compute once the prime factorisation 444 = 2²·3·37
       and generate all divisors in increasing order. */
    static uint64_t divisors[64];
    static size_t dcount = 0;

    if (dcount == 0) {
        /* factor 444 */
        const uint64_t TARGET = 444ULL;
        uint64_t tmp = TARGET;
        uint64_t primes[16];
        int exp[16];
        size_t pf = 0;

        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                primes[pf] = p;
                exp[pf] = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    exp[pf]++;
                }
                pf++;
            }
        }
        if (tmp > 1) {
            primes[pf] = tmp;
            exp[pf] = 1;
            pf++;
        }

        /* generate all divisors */
        dcount = 1;                 /* start with 1 */
        divisors[0] = 1ULL;

        for (size_t i = 0; i < pf; ++i) {
            uint64_t p = primes[i];
            int e = exp[i];
            size_t old = dcount;

            for (size_t j = 0; j < old; ++j) {
                uint64_t val = divisors[j];
                for (int k = 1; k <= e; ++k) {
                    val *= p;
                    divisors[dcount++] = val;
                }
            }
        }

        /* sort the array – only 12 elements, bubble sort is fine */
        for (size_t i = 0; i < dcount; ++i) {
            for (size_t j = i + 1; j < dcount; ++j) {
                if (divisors[i] > divisors[j]) {
                    uint64_t t = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = t;
                }
            }
        }
    }

    if (n == 0) return 0;           /* undefined for offset 0 */
    size_t idx = (size_t)(n - 1);
    if (idx < dcount) return (seq_t)divisors[idx];
    return 0;                       /* n beyond known range */
}
