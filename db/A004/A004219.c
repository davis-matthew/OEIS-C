/*
 * A004219  a(n) = floor(10*log_10(n)).
 * https://oeis.org/A004219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10*log_10(n)) = (number of decimal digits of n^10)
 *            - 1, computed exactly with multi-precision integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9682 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static int big_mul(uint32_t *r, const uint32_t *x, int lx, const uint32_t *y, int ly) {
  int i, j, len;
  memset(r, 0, sizeof(uint32_t) * (size_t)(lx + ly));
  for (i = 0; i < lx; i++) {
    uint64_t carry = 0;
    for (j = 0; j < ly; j++) {
      uint64_t t = (uint64_t)x[i] * y[j] + r[i + j] + carry;
      r[i + j] = (uint32_t)t;
      carry = t >> 32;
    }
    r[i + ly] = (uint32_t)carry;
  }
  len = lx + ly;
  while (len > 0 && r[len - 1] == 0) len--;
  return len;
}

seq_t a(idx_t n) {
  uint32_t base[2], acc[32], tmp[32];
  int lb, la, k, digits;
  base[0] = (uint32_t)n;
  base[1] = (uint32_t)(n >> 32);
  lb = base[1] ? 2 : 1;
  acc[0] = 1;
  la = 1;
  for (k = 0; k < 10; k++) {
    la = big_mul(tmp, acc, la, base, lb);
    memcpy(acc, tmp, sizeof(uint32_t) * (size_t)la);
  }
  digits = 0;
  while (la > 0) {
    uint64_t rem = 0;
    int i;
    for (i = la - 1; i >= 0; i--) {
      uint64_t cur = (rem << 32) | acc[i];
      acc[i] = (uint32_t)(cur / 10);
      rem = cur % 10;
    }
    while (la > 0 && acc[la - 1] == 0) la--;
    digits++;
  }
  return (seq_t)(digits - 1);
}
