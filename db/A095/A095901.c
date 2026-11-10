/*
 * A095901  A004001 (mod 2).
 * https://oeis.org/A095901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A004001(n) mod 2, where A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1)), A004001(1) =
 *            A004001(2) = 1
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 121 terms through n = 7501 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A095901_cache = NULL;
static idx_t A095901_len = 0;

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (n >= A095901_len) {
    idx_t new_len = n + 1;
    if (new_len < 3) new_len = 3;
    if (A095901_cache) free(A095901_cache);
    A095901_cache = (uint32_t *)calloc((size_t)new_len, sizeof(uint32_t));
    A095901_cache[1] = 1;
    A095901_cache[2] = 1;
    for (idx_t i = 3; i < new_len; i++) {
      idx_t p = (idx_t)A095901_cache[i - 1];
      A095901_cache[i] = A095901_cache[p] + A095901_cache[i - p];
    }
    A095901_len = new_len;
  }
  return (seq_t)(A095901_cache[n] & 1u);
}
