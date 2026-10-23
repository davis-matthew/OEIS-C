/*
 * A109992  Numbers n such that the string 99n is prime.
 * https://oeis.org/A109992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 9, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Helpers:   count_digits, isprime_big, mul128, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109992: numbers n such that the concatenation 99n is prime */
/* Implementation: brute force search with Miller‑Rabin primality test */
/* for arbitrary‑precision numbers. The function runs in O(n) time. */

#include <stdbool.h>
#include "../utils/count_digits.h"
#include "../utils/pow10.h"
#include "../utils/mul128.h"
#include "../utils/isprime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for offset 1 – return 0 for safety */

    idx_t found = 0;        /* how many valid n's found so far */
    idx_t cur    = 0;        /* current integer being tested */

    while (found < n) {
        cur++;                                 /* next candidate number      */
        int d = count_digits(cur);             /* number of decimal digits   */
        seq_t part = mul128((seq_t)99, pow10(d));/* 99 * 10^d                 */
        seq_t test = part + (seq_t)cur;         /* 99… concatenated with cur   */

        if (isprime_big(test))
            found++;                           /* increment count if prime    */
    }

    return (seq_t)cur;                           /* return the nth valid n      */
}
