/*
 * A018377  Divisors of 290.
 * https://oeis.org/A018377
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

/* Sequence: Sorted divisors of 290 (positive). */
#include <stdbool.h>

/* Static storage for the divisors: small ones up to sqrt(290),
   and the corresponding large ones.  */
static seq_t small[20];
static seq_t large[20];
static idx_t small_cnt = 0;
static idx_t large_cnt = 0;
static bool init_done = false;

/* Compute the divisors once. */
static void _init_divs(void) {
    if (init_done) return;
    const uint64_t N = 290;            // value whose divisors are needed
    idx_t i = 1;
    while ((uint64_t)i * (uint64_t)i <= N) {
        if (N % i == 0) {
            small[small_cnt++] = (seq_t)i;
            if (i != N / i) {
                large[large_cnt++] = (seq_t)(N / i);
            }
        }
        ++i;
    }
    init_done = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for indices < 1 */
    _init_divs();
    if (n <= small_cnt) return small[n - 1];
    if (n <= small_cnt + large_cnt) {
        /* large[] stores the large divisors in descending order,
           so reverse it to get ascending order. */
        return large[large_cnt - (n - small_cnt)];
    }
    return 0;  /* sequence is finite; values beyond the last divisor are 0 */
}
