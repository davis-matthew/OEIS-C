/*
 * A229879  Palindromic prime numbers == 5 (mod 9).
 * https://oeis.org/A229879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   compute the nth palindromic prime congruent to 5 mod 9 by
 *            brute‑force search over odd‑length palindromes (length 1, 3, 5,
 *            …)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 13, u64 n <= 993, u128 n <= 993,
 *            big any n
 * Helpers:   is_prime_big, pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rev_decimal.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* handle 1‑digit palindromes */
    idx_t count = 0;
    for (seq_t d = (seq_t)2; d <= (seq_t)9; ++d) {
        seq_t val = d;
        if (val % (seq_t)9 == (seq_t)5 && is_prime_big(val)) {
            ++count;
            if (count == n) return val; /* 5 is the only 1‑digit candidate */
        }
    }

    /* generate palindromes of odd length 3,5,7,… */
    for (idx_t k = 1; ; ++k) {
        seq_t pow10_k   = pow10(k);        /* 10^k      */
        seq_t pow10_k1  = pow10(k + 1);    /* 10^(k+1)  */
        seq_t start = pow10_k;             /* first i of this length */
        seq_t end   = pow10_k1 - (seq_t)1; /* last i of this length */

        for (seq_t i = start; i <= end; ++i) {
            seq_t rev = rev_decimal(i / (seq_t)10);          /* reverse left part */
            seq_t val = i * pow10_k + rev;                  /* full palindrome */
            if (val % (seq_t)9 == (seq_t)5 && is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
    }

    /* should never reach here */
    return (seq_t)0;
}
