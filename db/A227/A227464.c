/*
 * A227464  E.g.f. equals the series reversion of sin(x) / exp(x).
 * https://oeis.org/A227464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A227464
   Formula: for n>=1
      if n even: a(n) = n * Π_{j=1..(n-2)/2} (n^2 + (2j)^2)
      if n odd : a(n) = Π_{j=1..(n-1)/2} (n^2 + (2j-1)^2)
*/
seq_t a(idx_t n)
{
    // Valid for n >= 1. Behaviour for n < 1 is irrelevant.
    const seq_t sn = (seq_t)n;
    const seq_t sn_sq = sn * sn;

    if ((n & 1) == 0) {           // even n
        seq_t result = sn;         // start with n
        idx_t limit = (n - 2) / 2; // number of factors
        for (idx_t j = 1; j <= limit; ++j) {
            idx_t k = 2 * j;
            seq_t term = sn_sq + (seq_t)(k * k);
            result *= term;
        }
        return result;
    } else {                       // odd n
        seq_t result = 1;          // empty product defaults to 1
        idx_t limit = (n - 1) / 2;
        for (idx_t j = 1; j <= limit; ++j) {
            idx_t k = 2 * j - 1;
            seq_t term = sn_sq + (seq_t)(k * k);
            result *= term;
        }
        return result;
    }
}
