/*
 * A007372  Numbers k such that sigma(x) = k has exactly 3 solutions.
 * https://oeis.org/A007372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve-based search. Since sigma(x) >= x+1, every solution of
 *            sigma(x) = k has x < k, so sieve sigma(x) for x <= K, count how
 *            many x give each k <= K, and list those k with exactly 3
 *            solutions. Double K until n terms are found
 * Range:     u8 n <= 10, u16 n <= 1111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *a007372_cache = NULL;
static idx_t a007372_count = 0;

static void a007372_build(uint64_t K) {
  uint64_t *sg = (uint64_t *)malloc((size_t)(K + 1) * sizeof(uint64_t));
  unsigned char *cnt = (unsigned char *)malloc((size_t)(K + 1));
  uint64_t i, j, k;
  idx_t found = 0;
  if (sg == NULL || cnt == NULL) {
    free(sg);
    free(cnt);
    return;
  }
  memset(sg, 0, (size_t)(K + 1) * sizeof(uint64_t));
  memset(cnt, 0, (size_t)(K + 1));
  for (i = 1; i <= K; i++)
    for (j = i; j <= K; j += i)
      sg[j] += i;
  for (i = 1; i <= K; i++) {
    k = sg[i];
    if (k <= K && cnt[k] < 255) cnt[k]++;
  }
  for (k = 1; k <= K; k++)
    if (cnt[k] == 3) found++;
  free(a007372_cache);
  a007372_cache = (uint64_t *)malloc((size_t)(found + 1) * sizeof(uint64_t));
  a007372_count = 0;
  if (a007372_cache != NULL) {
    for (k = 1; k <= K; k++)
      if (cnt[k] == 3) a007372_cache[a007372_count++] = k;
  }
  free(sg);
  free(cnt);
}

seq_t a(idx_t n) {
  uint64_t K;
  if (n == 0) return (seq_t)0;
  if (a007372_cache != NULL && n <= a007372_count)
    return (seq_t)a007372_cache[n - 1];
  K = 1024;
  while (K < (uint64_t)n * 40) K *= 2;
  for (;;) {
    a007372_build(K);
    if (a007372_cache != NULL && n <= a007372_count)
      return (seq_t)a007372_cache[n - 1];
    K *= 2;
  }
}
