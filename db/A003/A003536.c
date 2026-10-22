/*
 * A003536  Divisors of 2^28 - 1.
 * https://oeis.org/A003536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^28 - 1 = 3*5*29*43*113*127
 *            (found by trial division up to sqrt)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t divs_a003536[128];
static idx_t cnt_a003536 = 0;
static int init_a003536 = 0;

static void a003536_init(void) {
    uint64_t num = (1ULL << 28) - 1ULL;
    uint64_t small[64], large[64];
    int ns = 0, nl = 0, i;
    uint64_t d;
    if (init_a003536) return;
    for (d = 1; d * d <= num; d++) {
        if (num % d == 0) {
            small[ns++] = d;
            if (num / d != d) large[nl++] = num / d;
        }
    }
    cnt_a003536 = 0;
    for (i = 0; i < ns; i++) divs_a003536[cnt_a003536++] = small[i];
    for (i = nl - 1; i >= 0; i--) divs_a003536[cnt_a003536++] = large[i];
    init_a003536 = 1;
}

seq_t a(idx_t n) {
    a003536_init();
    if (n >= 1 && n <= cnt_a003536) return (seq_t)divs_a003536[n - 1];
    return (seq_t)0;
}
