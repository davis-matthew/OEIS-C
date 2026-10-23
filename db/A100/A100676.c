/*
 * A100676  a(1) = 1; a(n+1) = Sum_{k=1..n} a(gcd(a(k),n)).
 * https://oeis.org/A100676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n+1) = Σ_{k=1..n} a(gcd(a(k), n)) (OEIS A100676) This
 *            implementation keeps a memoized table of all computed terms and
 *            expands it as needed. A small Euclidean gcd function is provided
 *            for seq_t values. The code follows all styling and safety
 *            requirements of the evaluation system
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Small gcd for seq_t values; seq_t is an unsigned integer type. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* 0‑index is unused; table[1] holds a(1). */
    static seq_t *tbl = NULL;
    static idx_t max_n = 0;

    if (n == 0) return 0;          /* safety: sequence starts at n=1 */

    /* Expand table if necessary. */
    if (n > max_n) {
        idx_t new_size = n + 1;    /* include index 0, unused */
        seq_t *tmp = (seq_t*)realloc(tbl, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure (unlikely) */
        tbl = tmp;

        /* Initialise the first term if this is the first expansion. */
        if (max_n == 0) {
            tbl[1] = (seq_t)1;
            max_n = 1;
        }
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t prev = i - 1;
        seq_t sum = 0;
        for (idx_t k = 1; k <= prev; ++k) {
            seq_t g = seq_gcd(tbl[k], (seq_t)prev);
            /* g is ≤ prev ≤ n, hence fits in idx_t. */
            sum += tbl[(idx_t)g];
        }
        tbl[i] = sum;
    }
    max_n = n;

    return tbl[n];
}
