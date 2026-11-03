/*
 * A109992  Numbers n such that the string 99n is prime.
 * https://oeis.org/A109992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 70 terms through n = 1847 (2026-11-01)
 * Helpers:   is_prime_seq, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109992: numbers n such that the concatenation 99n is prime */
/* Implementation: brute force search with Miller‑Rabin primality test */
/* for arbitrary‑precision numbers. The function runs in O(n) time. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdalign.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Helper: safe multiplication using 128‑bit intermediate when
   available and seq_t is a built‑in integer.  For arbitrary‑precision
   types the plain * operator is used. */
static inline seq_t mul128(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    return (seq_t)((unsigned __int128)(unsigned long long)a * b);
#else
    return a * b;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for offset 1 – return 0 for safety */

    idx_t found = 0;        /* how many valid n's found so far */
    idx_t cur    = 0;        /* current integer being tested */

    while (found < n) {
        cur++;                                 /* next candidate number      */
        int d = num_digits_idx(cur);             /* number of decimal digits   */
        seq_t part = mul128((seq_t)99, pow10(d));/* 99 * 10^d                 */
        seq_t test = part + (seq_t)cur;         /* 99… concatenated with cur   */

        if (is_prime_seq(test))
            found++;                           /* increment count if prime    */
    }

    return (seq_t)cur;                           /* return the nth valid n      */
}
