/*
 * A159765  Divisors of 1000000.
 * https://oeis.org/A159765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 1000000 (sorted in increasing order). */
/* a(n) gives the n‑th smallest divisor of 1000000 (offset 1). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are exactly 49 divisors of 10^6 = 2^6*5^6. */
    static seq_t divisors[49];
    static idx_t divisor_cnt = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t N = 1000000ULL;          /* 10^6 = 2^6 * 5^6 */
        seq_t big[49];                      /* store N/i for i > sqrt(N) */
        idx_t big_cnt = 0;

        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divisors[divisor_cnt++] = (seq_t)i;
                /* avoid duplicating the square root divisor
                   when i * i == N */
                if (i != N / i) {
                    big[big_cnt++] = (seq_t)(N / i);
                }
            }
        }
        /* Append the larger divisors in ascending order */
        for (idx_t j = 0; j < big_cnt; ++j) {
            divisors[divisor_cnt++] = big[big_cnt - 1 - j];
        }
        initialized = true;                /* ensure initialization only once */
    }

    if (n < 1 || n > divisor_cnt) return 0;
    return divisors[n - 1];
}
