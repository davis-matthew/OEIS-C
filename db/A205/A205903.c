/*
 * A205903  Number of (n+1)X3 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock the same.
 * https://oeis.org/A205903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A205903
 * Formula (empirical, valid for n>11):
 *   a(n) = 17*a(n‑1) – 86*a(n‑2) + 117*a(n‑3) + 90*a(n‑4) – 446*a(n‑5)
 *         + 619*a(n‑6) – 476*a(n‑7) + 234*a(n‑8) – 92*a(n‑9) + 24*a(n‑10)
 * Initial values (for 1 ≤ n ≤ 11):
 *   a(1)=369, a(2)=2943, a(3)=24045, a(4)=201324, a(5)=1712907,
 *   a(6)=14747754, a(7)=128014116, a(8)=1117395426, a(9)=9789931593,
 *   a(10)=85987653345, a(11)=756504850728
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;     /* cache of computed terms */
static idx_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;    /* allocated capacity of cache   */

seq_t a(idx_t n)
{
    /* Initial terms of the sequence (a(1) .. a(11)) */
    static const seq_t init[] = {
        369ULL,
        2943ULL,
        24045ULL,
        201324ULL,
        1712907ULL,
        14747754ULL,
        128014116ULL,
        1117395426ULL,
        9789931593ULL,
        85987653345ULL,
        756504850728ULL
    };
    const size_t init_len = sizeof init / sizeof init[0];

    if (n == 0) return 0;   /* undefined for n = 0, return 0 */

    /* Expand cache if necessary */
    if (n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n);
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = (size_t)n;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= init_len) {
            cache[i - 1] = init[i - 1];
        } else {
            seq_t res = 0;
            res += (seq_t)17 * cache[i - 2];
            res -= (seq_t)86 * cache[i - 3];
            res += (seq_t)117 * cache[i - 4];
            res += (seq_t)90 * cache[i - 5];
            res -= (seq_t)446 * cache[i - 6];
            res += (seq_t)619 * cache[i - 7];
            res -= (seq_t)476 * cache[i - 8];
            res += (seq_t)234 * cache[i - 9];
            res -= (seq_t)92 * cache[i - 10];
            res += (seq_t)24 * cache[i - 11];
            cache[i - 1] = res;
        }
    }
    cache_len = n;
    return cache[n - 1];
}
