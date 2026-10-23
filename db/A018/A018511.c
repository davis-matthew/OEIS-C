/*
 * A018511  Divisors of 546.
 * https://oeis.org/A018511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 546 (sorted ascending)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t cache[16];
static bool initialized = false;

static int cmp_seq_t(const void *p1, const void *p2) {
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Build the sorted list of divisors of 546 */
static void init_divisors(void) {
    const seq_t primes[4] = { (seq_t)2, (seq_t)3, (seq_t)7, (seq_t)13 };
    seq_t temp[16];
    int count = 0;
    for (idx_t mask = 0; mask < 16; ++mask) {
        seq_t prod = (seq_t)1;
        for (int j = 0; j < 4; ++j) {
            if ((mask >> j) & 1) prod *= primes[j];
        }
        temp[count++] = prod;
    }
    qsort(temp, count, sizeof(seq_t), cmp_seq_t);
    for (int i = 0; i < 16; ++i) cache[i] = temp[i];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for n=0 */
    if (!initialized) { init_divisors(); initialized = true; }
    if (n <= 16) return cache[n - 1];
    return (seq_t)0; /* undefined for n > 16 */
}
