/*
 * A003554  Divisors of 2^50 - 1.
 * https://oeis.org/A003554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of N = 2^50 - 1 in increasing order: factor N by trial
 *            division, build every product of prime powers, then sort
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 93, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t A3554_divs[256];
static int A3554_cnt = 0;

static void A3554_init(void) {
    uint64_t N = (((uint64_t)1) << 50) - 1;
    uint64_t m = N;
    int c, i, j;
    uint64_t p;
    A3554_divs[0] = 1;
    c = 1;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            {
                int old = c;
                uint64_t pw = 1;
                int k;
                for (k = 1; k <= e; k++) {
                    pw *= p;
                    for (j = 0; j < old; j++) A3554_divs[c++] = A3554_divs[j] * pw;
                }
            }
        }
    }
    if (m > 1) {
        int old = c;
        for (j = 0; j < old; j++) A3554_divs[c++] = A3554_divs[j] * m;
    }
    /* insertion sort */
    for (i = 1; i < c; i++) {
        uint64_t v = A3554_divs[i];
        j = i - 1;
        while (j >= 0 && A3554_divs[j] > v) {
            A3554_divs[j + 1] = A3554_divs[j];
            j--;
        }
        A3554_divs[j + 1] = v;
    }
    A3554_cnt = c;
}

seq_t a(idx_t n) {
    if (A3554_cnt == 0) A3554_init();
    if (n < 1 || n > (idx_t)A3554_cnt) return (seq_t)0;
    return (seq_t)A3554_divs[n - 1];
}
