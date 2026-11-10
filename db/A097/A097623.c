/*
 * A097623  a(n) = A097621(A097621(A097621(n))).
 * https://oeis.org/A097623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A097621(A097621(A097621(n))), where A097621 is
 *            multiplicative and maps p^e to its 1-based index in A000961
 *            (1,2,3,4,5,7,8,9,11,...)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* index of prime power q in A000961 (q=1 -> 1) */
static idx_t A097621_pp_index(idx_t q)
{
    if (q <= 1) return 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)q + 1, 1);
    idx_t cnt = 1; /* the number 1 */
    for (idx_t i = 2; i <= q; ++i) {
        if (!comp[i]) {
            if (i <= q / i) {
                for (idx_t j = i * i; j <= q; j += i) comp[j] = 1;
            }
            idx_t v = i;
            for (;;) {
                ++cnt;
                if (v > q / i) break;
                v *= i;
            }
        }
    }
    free(comp);
    return cnt;
}

static idx_t A097621_f(idx_t n)
{
    if (n <= 1) return 1;
    idx_t result = 1;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t q = 1;
            while (t % p == 0) { t /= p; q *= p; }
            result *= A097621_pp_index(q);
        }
    }
    if (t > 1) result *= A097621_pp_index(t);
    return result;
}

seq_t a(idx_t n)
{
    idx_t r = A097621_f(A097621_f(A097621_f(n)));
    return (seq_t)r;
}
