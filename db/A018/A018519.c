/*
 * A018519  Divisors of 560.
 * https://oeis.org/A018519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 560, sorted increasingly
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence has exactly 20 terms: the divisors of 560 = 2^4·5·7.
       They are generated once and cached for subsequent calls. */
    static seq_t divs[20];
    static bool initialized = false;

    if (!initialized) {
        /* generate all 20 divisors */
        const unsigned int primes[3] = {2, 5, 7};
        const unsigned int exps[3]  = {4, 1, 1};

        size_t idx = 0;
        for (unsigned int e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t val0 = 1;
            for (unsigned int i = 0; i < e0; ++i) val0 *= (seq_t)primes[0];
            for (unsigned int e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t val1 = val0;
                for (unsigned int i = 0; i < e1; ++i) val1 *= (seq_t)primes[1];
                for (unsigned int e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t val2 = val1;
                    for (unsigned int i = 0; i < e2; ++i) val2 *= (seq_t)primes[2];
                    divs[idx++] = val2;
                }
            }
        }

        /* sort the 20 values in ascending order (insertion sort, small array) */
        for (size_t i = 1; i < idx; ++i) {
            for (size_t j = i; j > 0 && divs[j] < divs[j - 1]; --j) {
                seq_t tmp = divs[j];
                divs[j]   = divs[j - 1];
                divs[j - 1] = tmp;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > 20)  /* off‑by‑one safety; values beyond the sequence are 0 */
        return 0;
    return divs[n - 1];
}
