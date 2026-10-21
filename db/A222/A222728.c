/*
 * A222728  Palindromic composite numbers starting with a digit 7.
 * https://oeis.org/A222728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 19, u64 n <= 1038,
 *            u128 n <= 1038, big any n
 * Helpers:   is_prime_big, pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = nth palindromic composite number (base 10) starting with digit 7.
 * The algorithm enumerates palindromes by length, builds only those whose first digit
 * is 7, checks composite status with is_prime_big, and returns the n-th composite.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/reverse.h"
#include "../utils/is_prime_big.h"

/* Build a palindrome from a half (first ceil(len/2) digits). */
static seq_t mk_palindrome(seq_t half, idx_t len)
{
    idx_t h = (len + 1) / 2;          /* digits in the first half             */
    if (len & 1) {                    /* odd length: drop middle digit of half */
        seq_t rev = reverse(half / 10);
        return half * pow10(h - 1) + rev;
    } else {                          /* even length: whole half reversed      */
        seq_t rev = reverse(half);
        return half * pow10(h) + rev;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 – behaviour below is irrelevant */

    idx_t found = 0;  /* how many composite palindromes with leading 7 have been seen */

    for (idx_t len = 2; ; ++len) {            /* palindrome length starts at 2 */
        idx_t h = (len + 1) / 2;              /* digits in first half */
        seq_t pow_h_1 = pow10(h - 1);         /* 10^(h-1)  (seq_t)          */
        idx_t start = 7 * (idx_t)pow_h_1;     /* smallest half with first digit 7 */
        seq_t pow_h = pow10(h);              /* 10^h  (seq_t) */
        idx_t end   = 8 * (idx_t)pow_h_1 - 1; /* largest half with first digit 7  */

        for (idx_t half = start; half <= end; ++half) {
            seq_t pal = mk_palindrome((seq_t)half, len);
            if (!is_prime_big(pal)) {        /* composite test */
                ++found;
                if (found == n)
                    return pal;
            }
        }
    }
}
