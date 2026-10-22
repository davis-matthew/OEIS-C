/*
 * A004560  Expansion of sqrt(5) in base 7.
 * https://oeis.org/A004560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 7^(n-1)) mod 7, digits computed by the
 *            schoolbook digit-by-digit square root in base 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4560_addsmall, a4560_cmp, a4560_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a4560_addsmall.h"
#include "../utils/a4560_cmp.h"
#include "../utils/a4560_sub.h"

static unsigned char *a4560_dig = NULL;
static idx_t a4560_len = 0;

static void a4560_build(idx_t N) {
    idx_t cap = N + 8;
    int *p = (int *)calloc(cap, sizeof(int));
    int *r = (int *)calloc(cap, sizeof(int));
    int *s = (int *)calloc(cap, sizeof(int));
    unsigned char *out = (unsigned char *)malloc(N);
    idx_t pl = 1, rl = 1, sl, k, i;

    p[0] = 2; r[0] = 1; out[0] = 2;
    for (k = 1; k < N; k++) {
        int carry = 0, x = 0;
        /* r = r * 49 (bring down the next pair of zero digits) */
        if (!(rl == 1 && r[0] == 0)) {
            memmove(r + 2, r, rl * sizeof(int));
            r[0] = 0; r[1] = 0;
            rl += 2;
        }
        /* s = 14*p + 1 = (2*p shifted one digit) + 1 */
        s[0] = 0;
        for (i = 0; i < pl; i++) {
            int v = 2 * p[i] + carry;
            s[i + 1] = v % 7;
            carry = v / 7;
        }
        sl = pl + 1;
        if (carry) s[sl++] = carry;
        s[0] = 1;
        /* (14p + x) * x = sum_{j=1..x} (14p + 2j - 1) */
        while (x < 6 && a4560_cmp(r, rl, s, sl) >= 0) {
            a4560_sub(r, &rl, s, sl);
            x++;
            a4560_addsmall(s, &sl, 2);
        }
        memmove(p + 1, p, pl * sizeof(int));
        p[0] = x;
        pl++;
        out[k] = (unsigned char)x;
    }
    free(p); free(r); free(s);
    if (a4560_dig != NULL) free(a4560_dig);
    a4560_dig = out;
    a4560_len = N;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (a4560_dig == NULL || n > a4560_len) {
        a4560_build(n + n / 2 + 16);
    }
    return (seq_t)a4560_dig[n - 1];
}
