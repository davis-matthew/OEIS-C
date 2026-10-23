/*
 * A018464  Divisors of 455.
 * https://oeis.org/A018464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th divisor of 455 (sorted in increasing order) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of the eight divisors of 455, computed on first call */
    static seq_t divs[8];
    static int initialized = 0;
    if (!initialized) {
        /* 455 = 5 × 7 × 13; generate all subset products */
        uint64_t primes[3] = {5, 7, 13};
        int cnt = 0;
        divs[cnt++] = (seq_t)1;                     /* divisor 1 */
        for (int i = 0; i < 3; ++i) {
            uint64_t p = primes[i];
            int curr = cnt;
            for (int j = 0; j < curr; ++j) {
                divs[cnt++] = divs[j] * (seq_t)p;
            }
        }
        /* sort the array in ascending order */
        for (size_t i = 1; i < (size_t)cnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        initialized = 1;
    }

    if (n == 0 || n > (idx_t)8)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
