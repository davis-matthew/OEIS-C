/*
 * A018546  Divisors of 606.
 * https://oeis.org/A018546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 606 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    const uint64_t M = 606ULL;
    static seq_t divisors[8];
    static size_t count = 0;

    if (count == 0) {                        /* first call: compute divisors */
        uint64_t rem = M;
        uint64_t primes[3];
        int exps[3];
        int sz = 0;

        /* trial division */
        uint64_t p = 2;
        while (p * p <= rem) {
            if (rem % p == 0) {
                primes[sz] = p;
                exps[sz] = 0;
                while (rem % p == 0) {
                    rem /= p;
                    exps[sz]++;
                }
                sz++;
            }
            p += (p == 2) ? 1 : 2;    /* skip even numbers after 2 */
        }
        if (rem > 1) {               /* remaining prime factor */
            primes[sz] = rem;
            exps[sz] = 1;
            sz++;
        }

        /* generate all divisors (2*3*101 → 8 divisors) */
        size_t idx = 0;
        for (int a = 0; a <= exps[0]; ++a) {
            seq_t valA = 1;
            for (int i = 0; i < a; ++i) valA *= (seq_t)primes[0];
            for (int b = 0; b <= exps[1]; ++b) {
                seq_t valB = valA;
                for (int i = 0; i < b; ++i) valB *= (seq_t)primes[1];
                for (int c = 0; c <= exps[2]; ++c) {
                    seq_t valC = valB;
                    for (int i = 0; i < c; ++i) valC *= (seq_t)primes[2];
                    divisors[idx++] = valC;
                }
            }
        }
        count = idx;

        /* sort ascending */
        for (size_t i = 0; i < count; ++i)
            for (size_t j = i + 1; j < count; ++j)
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
    }

    if (n >= 1 && (idx_t)n <= count)
        return divisors[(size_t)(n - 1)];
    return 0;                         /* out of range for this finite sequence */
}
