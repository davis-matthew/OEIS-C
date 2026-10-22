/*
 * A007989  Slowest increasing sequence with property that a(n) does not
 *          divide any term of form a(n+m*a(n)) for any m; this is the slowest
 *          increasing "non-difference-divisible" sequence.
 * https://oeis.org/A007989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   greedy search: a(0) = 2; a(n) is the least integer > a(n-1) such
 *            that for no i < n with a(i) | (n - i) does a(i) divide a(n)
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *vals = NULL;
static idx_t vals_len = 0;   /* number of computed terms */
static idx_t vals_cap = 0;

seq_t a(idx_t n) {
    if (n >= vals_len) {
        if (n + 1 > vals_cap) {
            idx_t nc = (n + 1) * 2;
            uint64_t *nv = (uint64_t *)malloc(nc * sizeof(uint64_t));
            for (idx_t i = 0; i < vals_len; i++) nv[i] = vals[i];
            free(vals);
            vals = nv;
            vals_cap = nc;
        }
        uint64_t *divs = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        for (idx_t pos = vals_len; pos <= n; pos++) {
            if (pos == 0) {
                vals[0] = 2;
                continue;
            }
            idx_t nd = 0;
            for (idx_t i = 0; i < pos; i++) {
                uint64_t ai = vals[i];
                if ((uint64_t)(pos - i) % ai == 0) divs[nd++] = ai;
            }
            uint64_t c = vals[pos - 1] + 1;
            for (;;) {
                int ok = 1;
                for (idx_t k = 0; k < nd; k++) {
                    if (c % divs[k] == 0) { ok = 0; break; }
                }
                if (ok) break;
                c++;
            }
            vals[pos] = c;
        }
        free(divs);
        vals_len = n + 1;
    }
    return (seq_t)vals[n];
}
