/*
 * A023437  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-7).
 * https://oeis.org/A023437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-7) Initial values: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for computed values and a flag indicating if a value
       has been computed */
    static seq_t *values = NULL;
    static uint8_t *seen   = NULL;
    static size_t cap     = 0;

    /* ensure we have space for indices up to n */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cap) {
        seq_t *tmpv = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
        uint8_t *tmps = (uint8_t*)realloc(seen,   new_cap);
        if (!tmpv || !tmps) {
            /* allocation failed – abort with an error code */
            return (seq_t)0;
        }
        /* initialize newly allocated seen entries to 0 */
        if (tmps + cap) {
            memset(tmps + cap, 0, (new_cap - cap) * sizeof(uint8_t));
        }
        values = tmpv;
        seen   = tmps;
        cap    = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (seen[i]) continue;          /* already computed */
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)0; break;
            case 1: val = (seq_t)1; break;
            case 2: val = (seq_t)1; break;
            case 3: val = (seq_t)2; break;
            case 4: val = (seq_t)3; break;
            case 5: val = (seq_t)5; break;
            case 6: val = (seq_t)8; break;
            default:
                val = values[i-1] + values[i-2] - values[i-7];
                break;
        }
        values[i] = val;
        seen[i]   = 1;
    }

    return values[n];
}
