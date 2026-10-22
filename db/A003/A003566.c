/*
 * A003566  Least number m such that 10^m == +- 1 mod (10n + 1).
 * https://oeis.org/A003566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 10^m == ±1 (mod 10n+1)
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  uint64_t mod = 10ULL * n + 1ULL;
  uint64_t power = 10ULL % mod;
  
  for (uint64_t m = 1ULL; m <= mod; m++) {
    if (power == 1ULL || power == mod - 1ULL) {
      return (seq_t)m;
    }
    unsigned __int128 temp = (unsigned __int128)power * 10ULL;
    power = (uint64_t)(temp % mod);
  }
  
  return (seq_t)0;
}
