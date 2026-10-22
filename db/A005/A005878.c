/*
 * A005878  Theta series of cubic lattice with respect to deep hole.
 * https://oeis.org/A005878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of representations of 8*n+3 as sum of three odd
 *            squares (with signs and order)
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x) {
  if (x == 0) return 0;
  seq_t root = x;
  while (1) {
    seq_t new_root = (root + x / root) / 2;
    if (new_root >= root) break;
    root = new_root;
  }
  return root;
}

seq_t a(idx_t n) {
  seq_t target = 8 * (seq_t)n + 3;
  seq_t count = 0;
  
  seq_t a_val;
  for (a_val = 1; a_val * a_val <= target; a_val += 2) {
    seq_t b_val;
    for (b_val = a_val; a_val * a_val + b_val * b_val <= target; b_val += 2) {
      seq_t c_squared = target - a_val * a_val - b_val * b_val;
      seq_t c_val = isqrt_seq(c_squared);
      
      if (c_val * c_val == c_squared && c_val >= b_val && c_val % 2 == 1) {
        seq_t perms;
        if (a_val == b_val && b_val == c_val) {
          perms = 1;
        } else if (a_val == b_val || b_val == c_val) {
          perms = 3;
        } else {
          perms = 6;
        }
        count += perms * 8;
      }
    }
  }
  
  return count;
}
