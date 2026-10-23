/*
 * A182105  Number of elements merged by bottom-up merge sort.
 * https://oeis.org/A182105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n≥1, a(n+1)=2*a(n) if a(n) has occurred an even
 *            number of times, otherwise a(n+1)=1
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* a(1) is the first non‑zero term */
    if (n == 1) return 1;

    seq_t val = 1;

#ifdef __cplusplus
    /* use a simple vector map for C++ builds (including arbitrary‑precision seq_t) */
    std::vector<seq_t> keys;
    std::vector<size_t> counts;
    keys.reserve((size_t)n);
    counts.reserve((size_t)n);

    for (size_t step = 1; step < n; ++step) {
        /* find or insert current value */
        size_t idx = 0;
        while (idx < counts.size() && keys[idx] != val) ++idx;

        if (idx == counts.size()) {          /* new value */
            keys.push_back(val);
            counts.push_back(0);
            idx = counts.size() - 1;
        }

        counts[idx]++;                       /* occurrence count includes this term */
        val = ((counts[idx] & 1) == 0) ? val * 2 : 1;   /* next term */
    }
#else
    /* use heap arrays for plain C builds */
    size_t distinct = 0;
    seq_t *keys = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    size_t *cnts = (size_t *)malloc((size_t)n * sizeof(size_t));

    for (size_t step = 1; step < n; ++step) {
        /* find or insert current value */
        size_t idx = 0;
        while (idx < distinct && keys[idx] != val) ++idx;

        if (idx == distinct) {               /* new value */
            keys[distinct] = val;
            cnts[distinct] = 0;
            distinct++;
        }

        cnts[idx]++;                         /* occurrence count includes this term */
        val = ((cnts[idx] & 1) == 0) ? val * 2 : 1;   /* next term */
    }

    free(keys);
    free(cnts);
#endif

    return val;  /* a(n) */
}
