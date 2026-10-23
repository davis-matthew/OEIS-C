/*
 * A151975  The number of ways one can flip seven consecutive tails (or heads)
 *          when flipping a coin n times.
 * https://oeis.org/A151975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - f(n+1) * where f(0) = 1 and for k ≥ 1 * f(k) =
 *            f(k-1)+f(k-2)+…+f(k-7) (terms with negative index are 0). * This
 *            is the number of binary strings of length n+1 that contain * at
 *            least one run of seven consecutive equal bits. The sequence *
 *            starts with a(0)=0, a(1)=0, …, a(7)=1, a(8)=3,...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* cache of f(k) values (k = 0 … computed_max) */
    static seq_t *f = NULL;
    static idx_t cap = 0;           /* allocated capacity */
    static idx_t computed_max = (idx_t)-1;  /* highest index computed so far */

    idx_t req = n + 1;              /* need f[req] */

    /* grow the array if necessary */
    if (cap <= req) {
        idx_t new_cap = (cap == 0) ? (req + 1) : cap;
        while (new_cap <= req) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        f = tmp;
        cap = new_cap;
    }

    /* initialise the f array up to index 0 (f(0)=1) */
    if (computed_max == (idx_t)-1) {
        f[0] = 1;
        computed_max = 0;
    }

    /* compute missing values up to the required index */
    for (idx_t i = computed_max + 1; i <= req; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k) {
            if (i >= k) sum += f[i - k];
        }
        f[i] = sum;
    }

    computed_max = req;

    seq_t p2  = pow2(n);
    seq_t ans = p2 - f[req];   /* a(n) = 2^n – f(n+1) */
    return ans;
}
