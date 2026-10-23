/*
 * A182686  a(n) = the largest n-digit number with exactly 13 divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the largest n‑digit number of the form p^12 (p = prime).
 *            Returns 0 if no such number exists
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10, pow_seq, safe_pow12_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/pow_seq.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/safe_pow12_le.h"

/* Binary search for floor(12th root) of X; uses safe_pow12_le to avoid
   overflow for 64‑bit builds. */
static seq_t root12_floor(seq_t X)
{
    idx_t k        = digit_len(X) - 1;       /* floor(log10(X))   */
    idx_t low_exp  = k / 12;                 /* floor(k/12)       */
    idx_t high_exp = low_exp + 1;
    seq_t low  = pow10(low_exp);
    seq_t high = pow10(high_exp);
    seq_t best = 0;

    while (low <= high)
    {
        seq_t mid  = (low + high) / 2;
        seq_t mid12 = safe_pow12_le(mid, X);

        if (mid12 <= X)          /* mid^12 fits, try larger */
        {
            best = mid;
            low  = mid + 1;
        }
        else                     /* mid^12 too big, try smaller */
        {
            if (mid == 0) break; /* safety against underflow    */
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Small n have no solutions.  */
    if (n < 4) return 0;

    /* Lower bound on p: ceil(10^(n-1)^(1/12)) */
    seq_t ten_n_minus1 = pow10(n - 1);
    seq_t low = root12_floor(ten_n_minus1);
    if (safe_pow12_le(low, ten_n_minus1) < ten_n_minus1) low += 1;

    /* Upper bound on p: floor((10^n-1)^(1/12)) */
    seq_t ten_n = pow10(n);
    seq_t high = root12_floor(ten_n);
    if (pow_seq(high, 12) >= ten_n) high -= 1;

    if (low > high) return 0;              /* no prime in range */

    /* Search downwards for the largest prime in [low, high] */
    seq_t p = high;
    while (true)
    {
        if (is_prime_big(p))
            return pow_seq(p, 12);
        if (p == low) break;
        --p;
    }
    return 0; // no prime found
}
