/*
 * A004195  The numbers not expressible as the sum of 4 distinct nonzero
 *          squares can be written D*4^n union E. This is D.
 * https://oeis.org/A004195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   sieve. Mark every N <= 2^17 that is a sum of 4 distinct nonzero
 *            squares. m is in D iff m is not divisible by 4 and 4^k*m is
 *            never such a sum. Since 4x is a sum whenever x is (double each
 *            root), checking 256*m is enough
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A4195_LIM (1L << 17)
#define A4195_MMAX (A4195_LIM / 256)

static long a4195_terms[A4195_MMAX + 1];
static long a4195_cnt = 0;
static bool a4195_done = false;

static void a4195_build(void) {
    unsigned char *ok = (unsigned char *)calloc((size_t)A4195_LIM + 1, 1);
    long a, b, c, d;
    long m;
    for (a = 1; 4 * a * a < A4195_LIM; a++) {
        long sa = a * a;
        for (b = a + 1; sa + 3 * b * b < A4195_LIM; b++) {
            long sb = sa + b * b;
            for (c = b + 1; sb + 2 * c * c < A4195_LIM; c++) {
                long sc = sb + c * c;
                for (d = c + 1; sc + d * d <= A4195_LIM; d++)
                    ok[sc + d * d] = 1;
            }
        }
    }
    for (m = 1; m <= A4195_MMAX; m++) {
        if (m % 4 != 0 && !ok[m * 256])
            a4195_terms[a4195_cnt++] = m;
    }
    free(ok);
    a4195_done = true;
}

seq_t a(idx_t n) {
    if (!a4195_done) a4195_build();
    if (n < 1 || (long long)n > (long long)a4195_cnt) return 0;
    return (seq_t)a4195_terms[(long long)n - 1];
}
