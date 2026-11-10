/*
 * A088742  Run lengths of A088023.
 * https://oeis.org/A088742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal consecutive terms in
 *            A088023 (A088023(1)=1, A088023(2)=2,
 *            A088023(2^k)=2*A088023(2^(k-1))-1 for k>=2, else
 *            A088023(n)=A088023(2t-n+1)+n-t with t=2^floor(log2 n))
 * Range:     u8 n <= 16385, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 115 terms through n = 9938 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t N = 2 * n + 16;
    for (;;) {
        seq_t *v = mem_new(seq_t, (size_t)N + 1);
        v[1] = 1;
        v[2] = 2;
        idx_t t = 2;
        for (idx_t i = 3; i <= N; i++) {
            if ((t << 1) <= i) t <<= 1;
            if (i == t) {
                v[i] = v[t >> 1] * 2 - 1;
            } else {
                idx_t m = (t << 1) - i + 1;
                v[i] = v[m] + (seq_t)(i - t);
            }
        }
        idx_t runs = 0;
        seq_t len = 0;
        bool found = false;
        seq_t result = 0;
        idx_t i = 1;
        while (i <= N) {
            idx_t j = i;
            while (j + 1 <= N && v[j + 1] == v[i]) j++;
            if (j == N) break; /* run may continue beyond N */
            runs++;
            if (runs == n) {
                len = (seq_t)(j - i + 1);
                result = len;
                found = true;
                break;
            }
            i = j + 1;
        }
        mem_free(v);
        if (found) return result;
        N *= 2;
    }
}
