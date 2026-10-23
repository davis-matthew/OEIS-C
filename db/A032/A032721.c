/*
 * A032721  Numbers k such that k prefixed by '4' and followed by '7' is
 *          prime.
 * https://oeis.org/A032721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 181, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers k such that the decimal number formed by prefixing a '4'
 *  and appending a '7' to k (i.e., 4·10^(d+1)+k·10+7, where d is the
 *  number of decimal digits of k) is prime.
 *  The function returns the n‑th such k with OEIS offset 1.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /*  n < 1 is undefined for this sequence (offset = 1) */
    if (n < 1) return 0;

    idx_t found = 0;      /* how many suitable k's we have found */
    idx_t kidx  = 0;      /* candidate integer, treated as idx_t for safety */
    seq_t result = 0;     /* value to return */

    while (found < n) {
        ++kidx;
        seq_t k = (seq_t)kidx;

        /* number of decimal digits of k */
        int d = digit_len(k);

        /* 4·10^(d+1) */
        idx_t exp = (idx_t)(d + 1);
        seq_t pow10_d = pow10(exp);

        /* construct M = 4·10^(d+1) + k·10 + 7 */
        seq_t M = ((seq_t)4) * pow10_d + k * 10 + 7;

        if (is_prime_big(M)) {
            ++found;
            if (found == n) {
                result = k;
                break;
            }
        }
    }

    return result;
}
