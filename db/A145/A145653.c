/*
 * A145653  a(n) = the length of the longest substring of digits that occurs
 *          both in the binary representation of the n-th prime and in the
 *          binary representation of the (n+1)th prime.
 * https://oeis.org/A145653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = length of the longest common binary substring of the n-th
 *            prime and the (n+1)-th prime. The function uses get_prime
 *            (provided by the utilities) to obtain the consecutive primes and
 *            then applies the classic longest-common-substring dynamic
 *            program
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the two consecutive primes.  get_prime is defined in
       the utilities; it returns the k-th prime as a uint32_t. */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Convert the primes to binary bit arrays with the most‑significant
       bit first.  The lengths are at most 32, so a 64‑element array is
       more than sufficient. */
    uint8_t bits1[64], bits2[64];
    size_t len1 = 0, len2 = 0;

    uint32_t tmp = p1;
    do {
        bits1[len1++] = tmp & 1u;
        tmp >>= 1;
    } while (tmp);
    for (size_t i = 0; i < len1 / 2; ++i) {
        uint8_t t = bits1[i];
        bits1[i] = bits1[len1 - 1 - i];
        bits1[len1 - 1 - i] = t;
    }

    tmp = p2;
    do {
        bits2[len2++] = tmp & 1u;
        tmp >>= 1;
    } while (tmp);
    for (size_t i = 0; i < len2 / 2; ++i) {
        uint8_t t = bits2[i];
        bits2[i] = bits2[len2 - 1 - i];
        bits2[len2 - 1 - i] = t;
    }

    /* Dynamic‑programming table for longest common substring.
       dp[prev][j] holds lengths for the previous row; dp[cur][j]
       holds lengths for the current row.  Only a few bytes are needed
       because the lengths are bounded by 32. */
    uint8_t dp[2][65] = {0};
    seq_t best = 0;

    for (size_t i = 1; i <= len1; ++i) {
        dp[1][0] = 0;
        for (size_t j = 1; j <= len2; ++j) {
            if (bits1[i - 1] == bits2[j - 1]) {
                dp[1][j] = dp[0][j - 1] + 1;
            } else {
                dp[1][j] = 0;
            }
            if (dp[1][j] > best) {
                best = (seq_t)dp[1][j];
            }
        }
        /* Copy the current row to the previous row for the next
           iteration. */
        for (size_t j = 0; j <= len2; ++j) {
            dp[0][j] = dp[1][j];
        }
    }

    return best;
}
