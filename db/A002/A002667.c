/*
 * A002667  Continued cotangent for Pi.
 * https://oeis.org/A002667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lehmer's continued cotangent: x(0) = Pi, a(n) = floor(x(n)),
 *            x(n+1) = (a(n)*x(n) + 1)/(x(n) - a(n)). Pi is computed with
 *            Machin's formula to adaptive precision, and the recurrence runs
 *            on rational interval bounds until both endpoints give the same
 *            a(k) for k = 0..n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   c2667_add, c2667_addw, c2667_atan, c2667_bl, c2667_cmp,
 *            c2667_divs, c2667_iszero, c2667_shl, c2667_shr, c2667_step,
 *            c2667_sub, c2667_subw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2667_add.h"
#include "../utils/c2667_addw.h"
#include "../utils/c2667_atan.h"
#include "../utils/c2667_bl.h"
#include "../utils/c2667_cmp.h"
#include "../utils/c2667_divs.h"
#include "../utils/c2667_iszero.h"
#include "../utils/c2667_shl.h"
#include "../utils/c2667_shr.h"
#include "../utils/c2667_step.h"
#include "../utils/c2667_sub.h"
#include "../utils/c2667_subw.h"

seq_t a(idx_t n) {
  size_t N = 64 + ((size_t)8 << (n < 40 ? n : 40));
  for (;;) {
    size_t L = 3 * N / 32 + 8, G = 64, i;
    uint32_t *blk = (uint32_t *)calloc(9 * L, sizeof(uint32_t));
    uint32_t *lP = blk, *lQ = blk + L, *uP = blk + 2 * L, *uQ = blk + 3 * L;
    uint32_t *aL = blk + 4 * L, *aU = blk + 5 * L, *R = blk + 6 * L;
    uint32_t *D = blk + 7 * L, *T = blk + 8 * L;
    idx_t k;
    int ok = 1;

    c2667_atan(lP, R, D, T, aL, L, 5, N + G);
    c2667_atan(uP, R, D, T, aL, L, 239, N + G);
    c2667_shl(lP, L, 4);
    c2667_shl(uP, L, 2);
    c2667_sub(lP, uP, L);
    c2667_shr(lP, L, G);
    memcpy(uP, lP, L * sizeof(uint32_t));
    c2667_subw(lP, L, 1);
    c2667_addw(uP, L, 2);
    memset(lQ, 0, L * sizeof(uint32_t));
    lQ[N / 32] = (uint32_t)1 << (N % 32);
    memcpy(uQ, lQ, L * sizeof(uint32_t));

    for (k = 0; k <= n; k++) {
      if (!c2667_step(lP, lQ, aL, R, D, T, L) ||
          !c2667_step(uP, uQ, aU, R, D, T, L) ||
          c2667_cmp(aL, aU, L) != 0) {
        ok = 0;
        break;
      }
    }
    if (ok) {
      seq_t res = 0;
      size_t top = L;
      while (top > 0 && aL[top - 1] == 0) top--;
      for (i = top; i-- > 0;) {
        int j;
        for (j = 3; j >= 0; j--) {
          res *= 256;
          res += (seq_t)((aL[i] >> (8 * j)) & 255u);
        }
      }
      free(blk);
      return res;
    }
    free(blk);
    N *= 2;
  }
}
