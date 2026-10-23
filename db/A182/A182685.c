/*
 * A182685  a(n) = the smallest n-digit number with exactly 13 divisors, a(n)
 *          = 0 if no such number exists.
 * https://oeis.org/A182685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A182685: a(n) = the smallest n‑digit number of the form p^12 (p prime).  */

#include <stdbool.h>
#include "../utils/pow10.h"     // pow10(idx_t)
#include "../utils/big_pow.h"   // big_pow(seq_t, seq_t)
#include "../utils/digit_len.h" // digit_len(seq_t)
#include "../utils/is_prime_big.h" // is_prime_big(seq_t)

/* 
   The function searches for the smallest prime p such that p^12 has exactly n digits.
   If no such prime exists (i.e., the interval of primes between 10^((n-1)/12) and 10^(n/12))
   is empty, the function returns 0.
*/
seq_t a(idx_t n) {
    /* n = 1..3 have no 13‑divisor number with that many digits */
    if (n < 4) return 0;

    /* Y = 10^(n-1)  →  lower bound for p^12 */
    seq_t Y = pow10(n - 1);

    /* Find ceil 12th root of Y via binary search:
       minimal integer p such that p^12 >= Y. */
    idx_t exp_half = (n - 1) / 12;

    seq_t low  = 1;
    seq_t high = pow10(exp_half + 1); /* high^12 >> Y guarantees high is an upper bound */

    seq_t exp12 = (seq_t)12;
    while (low < high) {
        seq_t mid = (low + high) / 2;

        seq_t mid_pow = big_pow(mid, exp12);  /* mid^12 */

        if (mid_pow >= Y)
            high = mid;
        else
            low = mid + 1;
    }

    seq_t p = low;   /* candidate prime starts at ceil_root(Y,12) */

    /* Find the next prime ≥ p */
    while (!is_prime_big(p)) p++;

    /* Compute candidate value p^12 */
    seq_t val = big_pow(p, exp12);

    /* Verify that val has exactly n digits.  If not, no such number. */
    return (digit_len(val) == n) ? val : 0;
}
