/*
 * A006902  a(n) = (2n)! * Sum_{k=0..n} (-1)^k * binomial(n,k) / (n+k)!.
 * https://oeis.org/A006902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n-2)*a(n) - (n^3+n^2-7*n+4)*a(n-1) + 2*(2*n-3)*(n-1)^3*a(n-2) =
 *            0, a(0) = 1, a(1) = 1, a(2) = 5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
  if (n == 0) return (seq_t)1;
  if (n == 1) return (seq_t)1;
  if (n == 2) return (seq_t)5;
  
  // Extend cache if needed
  if (n >= cache_len) {
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (new_cache == NULL) return 0;
    if (cache != NULL) {
      memcpy(new_cache, cache, cache_len * sizeof(seq_t));
      free(cache);
    }
    memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
    cache = new_cache;
    cache_len = new_len;
  }
  
  // Initialize base cases
  cache[0] = (seq_t)1;
  cache[1] = (seq_t)1;
  cache[2] = (seq_t)5;
  
  // Compute a(n) using recurrence
  for (idx_t i = 3; i <= n; i++) {
    if (cache[i] == 0) { // Not yet computed
      seq_t a_i_minus_1 = cache[i - 1];
      seq_t a_i_minus_2 = cache[i - 2];
      
      seq_t i_seq = (seq_t)i;
      seq_t i_minus_1_seq = (seq_t)(i - 1);
      seq_t i_minus_2_seq = (seq_t)(i - 2);
      
      seq_t coeff1 = i_seq * i_seq * i_seq + i_seq * i_seq - (seq_t)7 * i_seq + (seq_t)4;
      seq_t term1 = coeff1 * a_i_minus_1;
      
      seq_t coeff2 = (seq_t)2 * ((seq_t)2 * i_seq - (seq_t)3) * i_minus_1_seq * i_minus_1_seq * i_minus_1_seq;
      seq_t term2 = coeff2 * a_i_minus_2;
      
      seq_t numerator = term1 - term2;
      seq_t result = numerator / i_minus_2_seq;
      
      cache[i] = result;
    }
  }
  
  return cache[n];
}
