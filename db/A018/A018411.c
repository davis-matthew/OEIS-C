/*
 * A018411  Divisors of 357.
 * https://oeis.org/A018411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 357 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The prime factorization of 357 is 3 * 7 * 17.
       There are 2^3 = 8 divisors.  We compute them once and cache
       the sorted list. */
    static bool init = false;
    static seq_t divs[8];
    if (!init) {
        static const seq_t primes[3] = {3, 7, 17};
        seq_t tmp[8];
        size_t m = 0;
        for (idx_t e0 = 0; e0 <= 1; ++e0) {
            for (idx_t e1 = 0; e1 <= 1; ++e1) {
                for (idx_t e2 = 0; e2 <= 1; ++e2) {
                    seq_t prod = 1;
                    if (e0) prod *= primes[0];
                    if (e1) prod *= primes[1];
                    if (e2) prod *= primes[2];
                    tmp[m++] = prod;
                }
            }
        }
        /* Simple insertion sort for 8 elements. */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = tmp[i];
            size_t j = i;
            while (j > 0 && tmp[j - 1] > key) {
                tmp[j] = tmp[j - 1];
                --j;
            }
            tmp[j] = key;
        }
        for (size_t i = 0; i < 8; ++i) divs[i] = tmp[i];
        init = true;
    }
    if (n >= 1 && n <= 8) {
        return divs[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
