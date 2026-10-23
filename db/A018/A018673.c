/*
 * A018673  Divisors of 834.
 * https://oeis.org/A018673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 834 (in ascending order)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache of the 8 divisors of 834 */
    static seq_t divisors[8];
    static bool ready = false;

    if (!ready) {
        /* factorise 834 */
        uint64_t m = 834ULL;
        uint64_t fac[8];
        int exp[8];
        int pf = 0;

        uint64_t temp = m;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                fac[pf] = p;
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                exp[pf] = e;
                ++pf;
            }
        }
        if (temp > 1) {
            fac[pf] = temp;
            exp[pf] = 1;
            ++pf;
        }

        /* generate all divisors by multiplying prime powers */
        divisors[0] = (seq_t)1;
        int dcount = 1;                    /* current number of divisors */

        for (int i = 0; i < pf; ++i) {
            uint64_t p = fac[i];
            int e = exp[i];
            int old = dcount;
            for (int j = 0; j < old; ++j) {
                seq_t val = divisors[j];
                seq_t prod = val;
                for (int k = 1; k <= e; ++k) {
                    prod *= (seq_t)p;
                    if (dcount < 8) divisors[dcount++] = prod;
                }
            }
        }

        /* sort the divisors increasingly (simple bubble sort) */
        for (int i = 0; i < dcount - 1; ++i) {
            for (int j = i + 1; j < dcount; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        ready = true;
    }

    if (n >= 1 && n <= 8) {
        return divisors[n - 1];
    } else {
        /* undefined outside the factor list; return zero */
        return (seq_t)0;
    }
}
