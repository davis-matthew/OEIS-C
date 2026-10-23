/*
 * A105509  Numbers m such that 9 is the leading digit of the m-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th Fibonacci index whose leading decimal digit
   is 9.  To test the leading digit we use the logarithmic identity

        F_m ≈ φ^m / √5        with  φ = (1+√5)/2

   Let L = log10(φ) ≈ 0.20898764024997873
       S = log10(√5) ≈ 0.3494850021680094

   The leading digit of F_m is the integer part of 10^( {m·L - S} ),
   where {x} is the fractional part of x.  The digit 9 occurs exactly
   when

        {m·L - S}  ∈  [log10(9), 1)

   We avoid floating point arithmetic by scaling all logarithms by
   SCALE = 10¹².  The scaled constants are stored as unsigned 64‑bit
   integers and all intermediate products fit comfortably in
   unsigned __int128.  The search stops as soon as the required
   count is reached, which is O(m) where m ≈ 22 n for the average
   case and fast enough for n up to 10 000. */

#include <stdint.h>
#include <stdbool.h>

/* Scaled logarithm constants (scaled by 1 000 000 000 000). */
static const unsigned long long SCALE            = 1000000000000ULL;
static const unsigned long long LOG10_PHI_SCALE  = 208987640250ULL; /* 0.20898764024997873 × 1e12 */
static const unsigned long long LOG10_SQRT5_SCALE = 349485002168ULL; /* 0.3494850021680094 × 1e12 */
static const unsigned long long THR9            = 954242509439ULL; /* log10(9) × 1e12 */

/* Determine whether the leading decimal digit of Fibonacci(m) is 9.
   Uses only integer arithmetic. */
static bool has_leading_nine(idx_t m)
{
    /* m × LOG10_PHI_SCALE  (fits in 128 bits) */
    unsigned __int128 prod = (unsigned __int128)m * (unsigned __int128)LOG10_PHI_SCALE;
    unsigned __int128 mod  = prod % (unsigned __int128)SCALE;          /* fractional part × SCALE */

    unsigned __int128 f_scaled = mod + (unsigned __int128)SCALE - (unsigned __int128)LOG10_SQRT5_SCALE;
    if (f_scaled >= (unsigned __int128)SCALE)
        f_scaled -= (unsigned __int128)SCALE;                          /* wrap to [0, SCALE) */

    return f_scaled >= (unsigned __int128)THR9;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, but guard against illegal input */
    idx_t count = 0;
    idx_t m = 1;
    for (;; ++m) {
        if (has_leading_nine(m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
