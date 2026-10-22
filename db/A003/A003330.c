/*
 * A003330  Numbers that are the sum of 7 positive cubes.
 * https://oeis.org/A003330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve of sums of exactly 7 positive cubes up to 2600 (DP over
 *            number of cubes); every integer > 2408 is a term, so a(n) = L +
 *            (n - count) beyond the sieve limit L
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define A3330_L 2600

static int a3330_terms[A3330_L + 1];
static long long a3330_cnt = -1;

static void a3330_init(void) {
    static unsigned char r[8][A3330_L + 1];
    int k, s, c;
    memset(r, 0, sizeof(r));
    r[0][0] = 1;
    for (k = 1; k <= 7; k++) {
        for (s = 0; s <= A3330_L; s++) {
            for (c = 1; c * c * c <= s; c++) {
                if (r[k - 1][s - c * c * c]) { r[k][s] = 1; break; }
            }
        }
    }
    a3330_cnt = 0;
    for (s = 1; s <= A3330_L; s++)
        if (r[7][s]) a3330_terms[a3330_cnt++] = s;
}

seq_t a(idx_t n) {
    if (a3330_cnt < 0) a3330_init();
    if (n == 0) return (seq_t)0;
    if (n <= (idx_t)a3330_cnt)
        return (seq_t)(unsigned long long)a3330_terms[n - 1];
    return (seq_t)(unsigned long long)A3330_L + (seq_t)(n - (idx_t)a3330_cnt);
}
