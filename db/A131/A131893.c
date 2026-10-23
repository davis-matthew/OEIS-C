/*
 * A131893  a(n) is the number of shapes of balanced trees with constant
 *          branching factor 7 and n nodes. The node is balanced if the size,
 *          measured in nodes, of each pair of its children differ by at most
 *          one node.
 * https://oeis.org/A131893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1; a(7*n+1+m) = (7 choose m) * a(n+1)^m *
 *            a(n)^(7-m)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 22, u64 n <= 68, u128 n <= 89,
 *            big any n
 * Helpers:   ensure_cache, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* static cache holding previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_comp = 0;            /* highest index computed so far */
    static const seq_t comb[8] = {
        (seq_t)1, (seq_t)7, (seq_t)21, (seq_t)35,
        (seq_t)35, (seq_t)21, (seq_t)7, (seq_t)1
    };

    /* Make sure the cache is large enough for the requested index */
    ensure_cache(n, &cache, &cap);

    /* Initialise the base cases if not yet done */
    if (max_comp == 0) {
        cache[0] = seq_one();
        if (n > 0) {
            cache[1] = seq_one();
            max_comp = 1;
        }
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t t = (i - 1) / 7;
        idx_t m = (i - 1) - 7 * t;          /* 0 <= m <= 7 */

        seq_t part1 = cache[t + 1];          /* a(n+1) */
        seq_t part2 = cache[t];              /* a(n)   */

        seq_t pow1   = seq_pow(part1, (idx_t)m);
        seq_t pow2   = seq_pow(part2, (idx_t)(7 - m));

        seq_t term = seq_mul(seq_mul(comb[m], pow1), pow2);
        cache[i] = term;
    }

    max_comp = n;
    return cache[n];
}
