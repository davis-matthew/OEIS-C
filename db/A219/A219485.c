/*
 * A219485  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 3, 7, 10, 17, 27, 44,
 *          ....
 * https://oeis.org/A219485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 635, u16 n <= 14140, u32 n <= 14140, u64 n <= 14140,
 *            u128 n <= 14140, big any n
 * Validated: standard, pass, 117 terms through n = 9325 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of representations of n as a sum of distinct elements of the
   generalized Fibonacci sequence 3,7,10,17,27,44,….
   Computes the number of subsets of that sequence whose sum equals n
   using a standard subset‑sum dynamic programming algorithm. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: generate all fib‑like numbers ≤ seq_n.
   The sequence is defined by F0=3, F1=7, Fn=Fn-1+Fn-2. */
static size_t generate_fib_like(seq_t seq_n, seq_t *buf, size_t bufcap)
{
    size_t k = 0;
    if (seq_n >= (seq_t)3) buf[k++] = (seq_t)3;
    if (seq_n >= (seq_t)7) buf[k++] = (seq_t)7;
    while (k >= 2 && k < bufcap) {
        seq_t nxt = buf[k - 1] + buf[k - 2];
        if (nxt > seq_n) break;
        buf[k++] = nxt;
    }
    return k;                /* number of values produced */
}

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t for uniform handling in comparisons. */
    seq_t seq_n = (seq_t)n;

    /* Generate the generalized Fibonacci numbers up to n. */
    /* 25 is ample for n ≤ 10000 (about 15 terms). */
    seq_t fib[25];
    size_t fibcnt = generate_fib_like(seq_n, fib, sizeof fib / sizeof fib[0]);

    /* Allocate dynamic programming array with zero initialization. */
    seq_t *ways = mem_new(seq_t, (size_t)n + 1);
    if (!ways) return (seq_t)0;            /* allocation failure → 0 */
    ways[0] = (seq_t)1;                    /* empty subset sums to 0 */

    /* For each Fibonacci number, update the DP array in reverse order
       to enforce distinctness of elements. */
    for (size_t idx = 0; idx < fibcnt; ++idx) {
        idx_t val = (idx_t)fib[idx];
        if (val > n) continue;            /* should not happen, guard nonetheless */
        for (idx_t sum = n; sum >= val; --sum) {
            ways[sum] += ways[sum - val];
        }
    }

    seq_t result = ways[(size_t)n];
    mem_free(ways);
    return result;
}
