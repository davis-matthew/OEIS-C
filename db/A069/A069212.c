/*
 * A069212  a(n) = Sum_{k=1..n} 3^omega(k).
 * https://oeis.org/A069212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} 3^omega(k) (omega(k) = number of distinct
 *            prime factors)
 * Range:     u8 n <= 39, u16 n <= 3551, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) independently for each call. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* safe guard for n=0 */
    /* Allocate array for omega */
    uint8_t *omega = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!omega) return 0;                    /* allocation failure: return 0 */

    /* Sieve to count distinct prime factors */
    for (idx_t i = 2; i <= n; ++i) {
        if (omega[i] == 0) {                  /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                ++omega[j];
            }
        }
    }

    /* Accumulate the sum of 3^omega(k) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint8_t w = omega[k];
        seq_t val = 1;
        for (uint8_t t = 0; t < w; ++t) val *= (seq_t)3;  /* 3^w */
        sum += val;
    }

    free(omega);
    return sum;
}
