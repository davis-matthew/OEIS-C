/*
 * A177688  Numbers n such that (n+2)//n - (n+1) is prime, where // represents
 *          the concatenation of decimals.
 * https://oeis.org/A177688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer n ≥ 0 such that (n+2)*10^k – 1 is prime
 *            where k = number of decimal digits of n
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 32, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t N) {
    /* N is 1‑based index.  We iterate over non‑negative integers and
       count how many satisfy the prime condition.  When the count
       reaches N we have found the required term. */
    idx_t found = 0;
    for (idx_t cand = 0; ; ++cand) {
        seq_t cand_seq = (seq_t)cand;

        /* Number of decimal digits of cand.  If cand == 0, digit_len
           returns 1, which is correct because 0 has one digit. */
        int k = digit_len(cand_seq);

        /* 10^k, as a seq_t value. */
        seq_t pow10k = pow10((idx_t)k);

        /* The candidate prime value p = (n+2)*10^k – 1. */
        seq_t p = (cand_seq + 2) * pow10k - 1;

        /* Test primality.  The utility handles all the bit‑widths
           that seq_t can represent. */
        if (is_prime_big(p)) {
            ++found;
            if (found == N) {
                return cand_seq;
            }
        }
    }
}
