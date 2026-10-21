/*
 * A205010  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=C(2j-2,j-1).
 * https://oeis.org/A205010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that there exists j with 1≤j<k and n |
 *            (s(k)−s(j)), where s(j)=C(2j−2,j−1). The function computes this
 *            by iterating the values s(k) modulo n until a residue repeats
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 58, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* cache for residues up to MAX_CACHE = 10000; stamp technique avoids
   clearing the array each call. */
#define MAX_CACHE 10001
static uint64_t stamp = 1;
static uint64_t used[MAX_CACHE] = {0};

seq_t a(idx_t n) {
    seq_t mod_n = (seq_t)n;          /* modulus as seq_t */
    seq_t cur   = 1;                 /* T1 = C(0,0) = 1 */

    /* first residue */
    idx_t res = (idx_t)(cur % mod_n);
    stamp++;                           /* new traversal mark */
    if (used[res] == stamp)            /* should not happen, but guard */
        return 1;
    used[res] = stamp;

    for (idx_t k = 2; ; ++k) {
        /* cur = cur * (4*k-6) / (k-1)  -- exact integer */
        cur = cur * (seq_t)(4*k - 6);
        cur = cur / (seq_t)(k - 1);

        res = (idx_t)(cur % mod_n);
        if (used[res] == stamp) {
            /* first repeat of residue occurs at index k */
            return (seq_t)k;
        }
        used[res] = stamp;
    }
}
