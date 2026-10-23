/*
 * A018681  Divisors of 848.
 * https://oeis.org/A018681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 848 in increasing order (offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018681: Divisors of 848 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache of the 10 divisors of 848 */
    static bool initialized = false;
    static seq_t divisors[20];
    static idx_t dcount = 0;

    if (!initialized) {
        /* Factorise 848 = 2^4 * 53 */
        uint64_t N = 848;
        uint64_t tmp = N;
        uint64_t primes[20];
        unsigned exps[20];
        idx_t pcnt = 0;

        /* Trial‑division to get prime factors */
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                unsigned e = 0;
                while (tmp % p == 0) {
                    ++e;
                    tmp /= p;
                }
                primes[pcnt] = p;
                exps[pcnt] = e;
                ++pcnt;
            }
        }
        if (tmp > 1) {
            primes[pcnt] = tmp;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* Generate all divisors  */
        dcount = 1;
        divisors[0] = 1;             /* start with divisor 1 */

        for (idx_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            unsigned e = exps[i];
            idx_t old = dcount;     /* number of divisors already known */

            for (unsigned k = 1; k <= e; ++k) {
                uint64_t mult = 1;  /* p^k */
                for (unsigned t = 1; t <= k; ++t) mult *= p;

                /* Multiply each existing divisor by mult */
                for (idx_t j = 0; j < old; ++j) {
                    if (dcount < 20) {   /* safety check */
                        uint64_t val = (uint64_t)divisors[j] * mult;
                        divisors[dcount++] = (seq_t)val;
                    }
                }
            }
        }

        /* Sort divisors in increasing order (simple bubble sort) */
        for (idx_t i = 0; i < dcount - 1; ++i) {
            for (idx_t j = i + 1; j < dcount; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmpv = divisors[i];
                    divisors[i]  = divisors[j];
                    divisors[j]  = tmpv;
                }
            }
        }
        initialized = true;
    }

    if (n >= 1 && n <= dcount) {
        return divisors[n - 1];
    }
    return 0;
}
