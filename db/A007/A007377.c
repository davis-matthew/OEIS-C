/*
 * A007377  Numbers k such that the decimal expansion of 2^k contains no 0.
 * https://oeis.org/A007377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   search over k >= 0: 2^k is kept as a decimal digit array,
 *            doubled each step, and k is recorded whenever no digit is 0
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A007377_KMAX 10000

static idx_t a007377_terms[64];
static idx_t a007377_count = 0;
static bool a007377_done = false;

static void a007377_init(void) {
    int cap = A007377_KMAX / 3 + 16;
    unsigned char *d = (unsigned char *)malloc((size_t)cap);
    int len = 1;
    idx_t k;
    if (!d) return;
    memset(d, 0, (size_t)cap);
    d[0] = 1;
    for (k = 0; k <= A007377_KMAX; k++) {
        bool zero = false;
        int i;
        for (i = 0; i < len; i++) {
            if (d[i] == 0) { zero = true; break; }
        }
        if (!zero && a007377_count < 64) {
            a007377_terms[a007377_count++] = k;
        }
        /* double */
        {
            int carry = 0;
            for (i = 0; i < len; i++) {
                int v = d[i] * 2 + carry;
                d[i] = (unsigned char)(v % 10);
                carry = v / 10;
            }
            if (carry && len < cap) {
                d[len++] = (unsigned char)carry;
            }
        }
    }
    free(d);
    a007377_done = true;
}

seq_t a(idx_t n) {
    if (!a007377_done) a007377_init();
    if (n < 1 || n > a007377_count) return (seq_t)0;
    return (seq_t)a007377_terms[n - 1];
}
