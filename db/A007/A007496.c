/*
 * A007496  Numbers n such that the decimal expansions of 2^n and 5^n contain
 *          no 0's (probably 33 is last term).
 * https://oeis.org/A007496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, base
 * Formula:   search: a(n) = n-th k >= 0 such that neither 2^k nor 5^k has a
 *            digit 0 in decimal (digits computed exactly with base-10 digit
 *            arrays)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   a7496_mul, a7496_nozero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a7496_mul.h"
#include "../utils/a7496_nozero.h"

#define A7496_LIMIT 4000
#define A7496_MAXD 2900

static int a7496_terms[64];
static int a7496_count = -1;

static void a7496_init(void) {
    unsigned char *p2 = (unsigned char *)malloc(A7496_MAXD);
    unsigned char *p5 = (unsigned char *)malloc(A7496_MAXD);
    int l2 = 1, l5 = 1, k;
    memset(p2, 0, A7496_MAXD);
    memset(p5, 0, A7496_MAXD);
    p2[0] = 1;
    p5[0] = 1;
    a7496_count = 0;
    for (k = 0; k <= A7496_LIMIT; k++) {
        if (a7496_nozero(p2, l2) && a7496_nozero(p5, l5)) {
            if (a7496_count < 64) a7496_terms[a7496_count++] = k;
        }
        l2 = a7496_mul(p2, l2, 2);
        l5 = a7496_mul(p5, l5, 5);
    }
    free(p2);
    free(p5);
}

seq_t a(idx_t n) {
    if (a7496_count < 0) a7496_init();
    if (n < 1 || n > (idx_t)a7496_count) return (seq_t)0;
    return (seq_t)a7496_terms[n - 1];
}
