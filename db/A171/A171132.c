/*
 * A171132  The smallest exponent k such that the digit n appears at two
 *          adjacent places in the decimal representation of 2^k.
 * https://oeis.org/A171132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = smallest k such that digit (n-1) appears twice
 *            consecutively in the decimal representation of 2^k. This is
 *            found by a simple search
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   get_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/get_digits.h"

/* Maximum number of digits we expect in 2^k for k up to some modest bound.
   2^1000 has 302 digits, so 512 is safely large for the exponents needed
   for digits 0–9. */
#define MAX_DIGITS 512

/* Cache of already computed values: index 0..9 correspond to digits 0..9. */
static seq_t cache[10] = { ((seq_t)0), ((seq_t)0), ((seq_t)0), ((seq_t)0),
                           ((seq_t)0), ((seq_t)0), ((seq_t)0), ((seq_t)0),
                           ((seq_t)0), ((seq_t)0) };

/* Compute the smallest k for a given single decimal digit d (0 <= d <= 9). */
static seq_t compute_k_for_digit(int d)
{
    /* Invalid digit or already computed? */
    if (d < 0 || d > 9) return ((seq_t)0);
    if (cache[d] != ((seq_t)0)) return cache[d];

    uint8_t digits[MAX_DIGITS];
    size_t len;
    seq_t k = 0;

    /* Search increasing k until a double digit d is found. */
    for (idx_t exp = 1; ; ++exp) {
        seq_t val = pow2(exp);
        get_digits(val, digits, &len);
        for (size_t i = 0; i + 1 < len; ++i) {
            if (digits[i] == (uint8_t)d && digits[i + 1] == (uint8_t)d) {
                k = exp;   /* found */
                break;
            }
        }
        if (k != 0) break;
    }

    cache[d] = k;
    return k;
}

/* Main sequence function.  idx_t n is 1-based as per OEIS offset. */
seq_t a(idx_t n)
{
    /* Map n -> digit: n=1 corresponds to digit 0, n=10 to digit 9. */
    int digit = (int)(n - 1);
    if (digit < 0 || digit > 9) return ((seq_t)0);  /* out of bounds */
    return compute_k_for_digit(digit);
}
