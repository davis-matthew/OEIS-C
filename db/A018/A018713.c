/*
 * A018713  Divisors of 904.
 * https://oeis.org/A018713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 904 (A018713)
   a(n) is the n‑th smallest positive divisor of 904.  The divisors of 904
   are 1, 2, 4, 8, 113, 226, 452, 904.  For n > 8 the sequence is undefined
   and the function returns 0. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are only 8 divisors of 904 = 2^3 * 113.  We compute them once
       and cache them. */
    static seq_t d[8];
    static bool ready = false;

    if (!ready) {
        size_t idx = 0;
        /* Enumerate exponents of 2 (0..3) and 113 (0..1). */
        for (size_t e2 = 0; e2 <= 3; ++e2) {
            seq_t pow2 = (seq_t)1 << e2;          /* 2^e2 */
            for (size_t e3 = 0; e3 <= 1; ++e3) {
                seq_t pow113 = e3 == 0 ? (seq_t)1 : (seq_t)113;
                d[idx++] = pow2 * pow113;
            }
        }
        /* Sort the divisors in increasing order (insertion sort). */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = d[i];
            size_t j = i;
            while (j > 0 && d[j - 1] > key) {
                d[j] = d[j - 1];
                --j;
            }
            d[j] = key;
        }
        ready = true;
    }

    if (n >= 1 && n <= 8)
        return d[(size_t)(n - 1)];
    return (seq_t)0;   /* undefined for n > 8 */
}
