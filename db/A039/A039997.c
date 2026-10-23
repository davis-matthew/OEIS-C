/*
 * A039997  Number of distinct primes which occur as substrings of the digits
 *          of n.
 * https://oeis.org/A039997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct prime substrings of the decimal
 *            representation of n
 * Range:     u8 n <= 4050, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digits.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Convert n to a decimal character array */
    int dlen = digits(n);
    size_t len = (size_t)dlen;
    char s[32];
    idx_t tmp = n;
    for (size_t i = 0; i < len; ++i) {
        s[len - i - 1] = (char)('0' + (tmp % 10));
        tmp /= 10;
    }

    /* Array to store distinct primes found (max 256 distinct primes for any n < 2^64) */
    idx_t uniq[256];
    size_t uniq_count = 0;

    /* Enumerate all substrings */
    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + (seq_t)(s[j] - '0');
            if (val < 2) continue;               /* skip 0 and 1 */
            idx_t v = (idx_t)val;                /* safe: val ≤ n < 2^64 */
            if (!is_prime(v)) continue;          /* skip composites */

            /* Check for duplicate */
            bool dup = false;
            for (size_t k = 0; k < uniq_count; ++k) {
                if (uniq[k] == v) { dup = true; break; }
            }
            if (!dup) {
                uniq[uniq_count++] = v;
                if (uniq_count == 256) break;    /* defensive upper bound */
            }
        }
        if (uniq_count == 256) break;
    }
    return (seq_t)uniq_count;
}
