/*
 * A007599  a(n+1) = a(n)+a(a(a(..(n-1)..))), depth [ n/2 ].
 * https://oeis.org/A007599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a^k(n-2), where k = floor(n/2) and a^k denotes a
 *            composed k times with a^0(x) = x
 * Range:     u8 n <= 282, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n) {
  static seq_t *memo = NULL;
  static bool *computed = NULL;
  static idx_t memo_len = 0;
  
  if (memo == NULL) {
    memo_len = 1024;
    memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
    computed = (bool *)malloc(memo_len * sizeof(bool));
    if (memo == NULL || computed == NULL) return 0;
    memset(computed, false, memo_len * sizeof(bool));
  }
  
  if (n >= memo_len) {
    idx_t newlen = ((n / 1024) + 1) * 1024;
    seq_t *newmemo = (seq_t *)realloc(memo, newlen * sizeof(seq_t));
    bool *newcomputed = (bool *)realloc(computed, newlen * sizeof(bool));
    if (newmemo == NULL || newcomputed == NULL) return 0;
    memo = newmemo;
    computed = newcomputed;
    memset(computed + memo_len, false, (newlen - memo_len) * sizeof(bool));
    memo_len = newlen;
  }
  
  if (computed[n]) {
    return memo[n];
  }
  
  seq_t result;
  if (n == 0) {
    result = 1;
  } else if (n == 1) {
    result = 0;
  } else {
    idx_t depth = n / 2;
    seq_t val = (seq_t)(n - 2);
    
    for (idx_t d = 0; d < depth; d++) {
      if (val == 0) {
        idx_t remaining = depth - d;
        val = (remaining % 2 == 1) ? 1 : 0;
        break;
      } else if (val == 1) {
        idx_t remaining = depth - d;
        val = (remaining % 2 == 1) ? 0 : 1;
        break;
      }
      val = a((idx_t)val);
    }
    
    result = a(n - 1) + val;
  }
  
  memo[n] = result;
  computed[n] = true;
  return result;
}
