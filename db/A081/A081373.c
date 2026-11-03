/*
 * A081373  Number of values of k, 1 <= k <= n, with phi(k) = phi(n), where
 *          phi is Euler totient function, A000010.
 * https://oeis.org/A081373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 1 ≤ k ≤ n | φ(k) = φ(n) }
 * Range:     u8 n <= 395, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 116 terms through n = 9584 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* defined only for n ≥ 1, safe guard */
    size_t sz = (size_t)n + 1;               /* indices 0..n inclusive */

    /* Allocate arrays for φ and a composite flag */
    seq_t *phi = mem_new(seq_t, sz);
    if (!phi) return 0;
    mem_set(phi, 0, sz * sizeof(seq_t));

    bool *comp = (bool*)malloc(sz * sizeof(bool));
    if (!comp) { mem_free(phi); return 0; }
    memset(comp, 0, sz * sizeof(bool));

    /* φ(0) = 0, φ(1) = 1 */
    if (sz > 1) phi[1] = 1;
    for (size_t i = 2; i < sz; ++i) phi[i] = (seq_t)i;  /* start with φ(i)=i */

    /* Sieve to compute Euler’s totient for all i ≤ n */
    for (size_t i = 2; i < sz; ++i) {
        if (!comp[i]) {                      /* i is prime */
            seq_t sm = phi[i] - phi[i] / (seq_t)i;   /* φ(i) = i-1       */
            phi[i] = sm;
            for (size_t j = i + i; j < sz; j += i) {
                comp[j] = true;
                seq_t tmp = phi[j];
                phi[j] = tmp - tmp / (seq_t)i;       /* φ(j) *= (i-1)/i */
            }
        }
    }

    seq_t target = phi[n];
    seq_t count = 0;
    for (size_t k = 1; k <= n; ++k) {
        if (phi[k] == target) ++count;
    }

    mem_free(phi);
    free(comp);
    return count;
}
