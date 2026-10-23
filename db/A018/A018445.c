/*
 * A018445  Divisors of 423.
 * https://oeis.org/A018445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all divisors of 423 (3²·47), sorted in increasing order */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool ready = false;

    if (!ready) {
        /* 423 = 3² · 47¹ -> 2+1 = 3 choices for the power of 3,
           1+1 = 2 choices for the power of 47 -> 6 divisors */
        const seq_t primes[2] = { (seq_t)3, (seq_t)47 };
        const idx_t  exps[2]  = { 2, 1 };
        size_t idx = 0;

        for (size_t e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t p3 = 1;
            for (size_t i = 0; i < e0; ++i) p3 *= primes[0];

            for (size_t e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t p47 = 1;
                for (size_t j = 0; j < e1; ++j) p47 *= primes[1];
                divisors[idx++] = p3 * p47;
            }
        }

        /* Bubble‑sort the 6 elements */
        for (size_t i = 1; i < 6; ++i)
            for (size_t j = 0; j < i; ++j)
                if (divisors[j] > divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        ready = true;
    }

    if (n < 1 || n > 6) return 0;
    return divisors[(size_t)(n - 1)];
}
