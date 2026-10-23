/*
 * A018430  Divisors of 392.
 * https://oeis.org/A018430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 392: a(n) = the n-th divisor of 392 in ascending order */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* generate divisors of 392 (2^3 * 7^2) only once */
    static seq_t divisors[12];
    static bool init = false;
    if (!init) {
        /* enumerate exponent pairs (e2, e7) */
        seq_t vals[12];
        unsigned i = 0;
        for (unsigned e2 = 0; e2 <= 3; ++e2) {
            seq_t pow2 = (seq_t)1 << e2;          /* 2^e2 */
            for (unsigned e7 = 0; e7 <= 2; ++e7) {
                seq_t pow7 = 1;
                for (unsigned k = 0; k < e7; ++k) pow7 *= (seq_t)7;
                vals[i++] = pow2 * pow7;
            }
        }
        /* simple insertion sort (12 elements) */
        for (unsigned i = 1; i < 12; ++i) {
            seq_t key = vals[i];
            unsigned j = i;
            while (j > 0 && vals[j - 1] > key) {
                vals[j] = vals[j - 1];
                --j;
            }
            vals[j] = key;
        }
        for (unsigned i = 0; i < 12; ++i) divisors[i] = vals[i];
        init = true;
    }
    if (n == 0) return 0;                     /* no term for n=0 */
    if (n > 12) return 0;                     /* undefined beyond the 12 divisors */
    return divisors[n - 1];
}
