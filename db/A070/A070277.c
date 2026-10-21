/*
 * A070277  Smallest prime ending in n, or 0 if no such prime exists.
 * https://oeis.org/A070277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1028, u32 n <= 3392, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070277:
   a(n) = the smallest prime ending with the decimal representation of n,
   or 0 if no such prime exists.  The function searches over numbers of
   the form n + k·10^d, where d is the number of digits in n.  It
   immediately returns 0 for suffixes that cannot be prime
   (even last digits other than 2 and odd last digits 5).  For all other
   cases it performs a fast prime test on each candidate. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/num_digits_idx.h"

/* Euclidean GCD for idx_t (assumed to be unsigned).  */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0U) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;          /* safety for index 0 */

    /* Fast elimination of impossible suffixes. */
    uint8_t last = (uint8_t)(n % 10U);
    if (last == 0U)
        return (seq_t)0;
    if (last == 2U)
        return (n == 2U) ? (seq_t)2 : (seq_t)0;
    if (last == 4U || last == 6U || last == 8U)
        return (seq_t)0;
    if (last == 5U)
        return (n == 5U) ? (seq_t)5 : (seq_t)0;

    /* Determine the modulus 10^d.  */
    idx_t d = num_digits_idx(n);
    idx_t mod_idx = 1U;
    for (idx_t i = 0; i < d; ++i)
        mod_idx *= 10U;

    /* If gcd(n,10^d) != 1, no prime in this arithmetic progression. */
    if (gcd_idx(n, mod_idx) != 1U)
        return (seq_t)0;

    /* Convert modulus to seq_t for arithmetic on candidates. */
    seq_t mod_seq = (seq_t)mod_idx;
    seq_t candidate = (seq_t)n;

    /* Search for the smallest prime in the progression. */
    while (true) {
        if (candidate >= (seq_t)2 &&
            is_prime_big(candidate))
            return candidate;

        candidate += mod_seq;
    }
}
