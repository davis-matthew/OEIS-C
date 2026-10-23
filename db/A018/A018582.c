/*
 * A018582  Divisors of 666.
 * https://oeis.org/A018582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 666 in increasing order.  */
#include <stdbool.h>
#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of bounds */

    /* Prime factorisation of 666: 2¹·3²·37¹ */
    static const seq_t primes[3]  = { 2, 3, 37 };
    static const idx_t  exps[3]   = { 1, 2, 1 };

    static bool      init   = false;
    static seq_t     divs[12];
    static const idx_t num_divs = 12;

    if (!init) {
        /* Generate all divisors */
        size_t pos = 0;
        for (idx_t e1 = 0; e1 <= exps[0]; ++e1)
            for (idx_t e2 = 0; e2 <= exps[1]; ++e2)
                for (idx_t e3 = 0; e3 <= exps[2]; ++e3) {
                    seq_t val = (seq_t)1;
                    for (idx_t i = 0; i < e1; ++i) val *= primes[0];
                    for (idx_t i = 0; i < e2; ++i) val *= primes[1];
                    for (idx_t i = 0; i < e3; ++i) val *= primes[2];
                    divs[pos++] = val;
                }

        /* Simple insertion sort (12 elements only) */
        for (size_t i = 1; i < num_divs; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        init = true;
    }

    if (n > num_divs) return (seq_t)0;   /* undefined beyond the finite set */
    return divs[n - 1];
}
