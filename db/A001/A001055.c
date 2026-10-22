/*
 * A001055  The multiplicative partition function: number of ways of factoring
 *          n with all factors greater than 1 (a(1) = 1 by convention).
 * https://oeis.org/A001055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, core
 * Formula:   a(n) = number of multiplicative partitions of n; recursive with
 *            memoization
 * Range:     u8 n <= 2879, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  idx_t n;
  idx_t minDiv;
  seq_t result;
} Memo;

static Memo *memo = NULL;
static idx_t memo_size = 0;
static idx_t memo_capacity = 0;

static seq_t count_partitions(idx_t n, idx_t minDiv);

static bool lookup_memo(idx_t n, idx_t minDiv, seq_t *out) {
  for (idx_t i = 0; i < memo_size; i++) {
    if (memo[i].n == n && memo[i].minDiv == minDiv) {
      *out = memo[i].result;
      return true;
    }
  }
  return false;
}

static void store_memo(idx_t n, idx_t minDiv, seq_t result) {
  if (memo_size >= memo_capacity) {
    memo_capacity = memo_capacity * 2 + 1;
    memo = (Memo *)realloc(memo, memo_capacity * sizeof(Memo));
  }
  memo[memo_size].n = n;
  memo[memo_size].minDiv = minDiv;
  memo[memo_size].result = result;
  memo_size++;
}

static seq_t count_partitions(idx_t n, idx_t minDiv) {
  if (n == 1) return 1;
  
  seq_t cached;
  if (lookup_memo(n, minDiv, &cached)) return cached;
  
  seq_t result = 0;
  for (idx_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      if (d >= minDiv && d > 1) {
        result += count_partitions(n / d, d);
      }
      idx_t d2 = n / d;
      if (d2 >= minDiv && d2 > 1 && d2 != d) {
        result += count_partitions(n / d2, d2);
      }
    }
  }
  
  store_memo(n, minDiv, result);
  return result;
}

seq_t a(idx_t n) {
  if (n == 1) return 1;
  return count_partitions(n, 2);
}
