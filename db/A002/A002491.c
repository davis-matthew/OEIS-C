/*
 * A002491  Smallest number of stones in Tchoukaillon (or Mancala, or
 *          Kalahari) solitaire that make use of n-th hole.
 * https://oeis.org/A002491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = successive rounding up to multiples: start with n, then
 *            round up to multiples of n-1, n-2, ..., 1
 * Range:     u8 n <= 27, u16 n <= 454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t value = (seq_t)n;
    
    for (idx_t k = n - 1; k >= 1; k--) {
        seq_t k_seq = (seq_t)k;
        // Round up value to next multiple of k_seq using ceiling division
        value = ((value + k_seq - 1) / k_seq) * k_seq;
    }
    
    return value;
}
