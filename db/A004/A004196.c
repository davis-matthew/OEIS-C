/*
 * A004196  The numbers not expressible as the sum of 4 distinct nonzero
 *          squares can be written D*4^n union E. This is E.
 * https://oeis.org/A004196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   sieve. Mark every s <= 32000 that is a sum of 4 distinct nonzero
 *            squares. E = the m <= 500 that are not such a sum but where
 *            m*4^k is one for some k in 1..3. Terms are listed in increasing
 *            order (finite sequence, 16 terms)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MLIM 500
#define LIM (MLIM * 64)

static uint64_t E_terms[MLIM];
static int E_count = -1;

static void build(void) {
    unsigned char *rep = (unsigned char *)calloc(LIM + 1, 1);
    uint64_t a, b, c, d, m;
    int k;
    for (a = 1; a * a + (a + 1) * (a + 1) + (a + 2) * (a + 2) + (a + 3) * (a + 3) <= LIM; a++)
        for (b = a + 1; a * a + b * b + (b + 1) * (b + 1) + (b + 2) * (b + 2) <= LIM; b++)
            for (c = b + 1; a * a + b * b + c * c + (c + 1) * (c + 1) <= LIM; c++) {
                uint64_t s3 = a * a + b * b + c * c;
                for (d = c + 1; s3 + d * d <= LIM; d++) rep[s3 + d * d] = 1;
            }
    E_count = 0;
    for (m = 1; m <= MLIM; m++) {
        if (rep[m]) continue;
        for (k = 1; k <= 3; k++) {
            if (rep[m << (2 * k)]) {
                E_terms[E_count++] = m;
                break;
            }
        }
    }
    free(rep);
}

seq_t a(idx_t n) {
    if (E_count < 0) build();
    if (n < 1 || n > (idx_t)E_count) return (seq_t)0;
    return (seq_t)E_terms[n - 1];
}
