/*
 * A120197  a(1)=4; a(n)=floor((35+sum(a(1) to a(n-1)))/8).
 * https://oeis.org/A120197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4; a(n)=floor((35+Σ_{i=1}^{n-1} a(i))/8) (for n≥1)
 * Range:     u8 n <= 36, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour for 0 is irrelevant */

    /* static memoised state */
    static seq_t  *vals  = NULL;  /* array of computed a(i) */
    static size_t  cap   = 0;     /* allocated capacity (in elements) */
    static idx_t   cur   = 0;     /* last index computed (>=0) */
    static unsigned long long sum64 = 0;   /* Σ_{i=1}^{cur} a(i) in 64‑bit */

    /* enlarge array if needed */
    if ((size_t)n > cap) {
        size_t needed = (size_t)n;
        size_t new_cap = cap ? cap << 1 : needed;
        if (new_cap < needed) new_cap = needed << 1;  /* guard overflow */
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        vals = tmp;
        cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cur + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 4;
        } else {
            /* (35 + sum)/8, all values are non‑negative */
            val = (seq_t)((35ULL + sum64) / 8ULL);
        }
        vals[k - 1] = val;
        sum64 += (unsigned long long)val;
    }

    cur = n;
    return vals[n - 1];
}
