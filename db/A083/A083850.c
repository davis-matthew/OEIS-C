/*
 * A083850  Exponent of highest power of 11 dividing the n-th decimal
 *          palindrome; a(0) = 1.
 * https://oeis.org/A083850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   exp_of_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the exponent of the highest power of 11 dividing the n‑th
    decimal palindrome,
    with a(0) = 1 (convention for the palindrome 0).  */

/*  We compute the n‑th palindrome directly and then count the
    factors of 11.  No tables of values are stored; the algorithm
    runs in O(log10(n)) time, negligible for n up to 10 000.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/exp_of_11.h"

/* Helper: reverse the decimal digits of a 64‑bit integer. */
static inline uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* Pre‑compute powers of ten up to 10^20 (more than enough for 64‑bit
   palindromes).  Index 0 holds 1 = 10^0.  */
static uint64_t pow10[21] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL,
    100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL
};

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* palindrome 0 */

    /*  The n‑th palindrome after 0 has index n-1 in the
        1‑based list of non‑zero palindromes.  */
    idx_t m = (idx_t)(n - 1);          /* offset within the list of non‑zero palindromes */

    /*  Find the length L of the palindrome.  Count palindromes of each
        length until we exceed m.  */
    int len = 1;
    idx_t cnt = 9;                     /* number of len‑digit palindromes when len>1;
                                         for len==1 it is also 9 (1..9) */
    while (m >= cnt) {
        m -= cnt;                      /* skip a whole block of length len */
        ++len;
        /*  For length len, the count equals 9 * 10^((len-1)/2).  */
        idx_t half = (len + 1) / 2;    /* ceil(len/2) */
        cnt = 9ULL * pow10[half - 1];
    }

    /*  Now len is the length of the desired palindrome and m
        (>=0) is its zero‑based index among palindromes of this length.  */
    uint64_t pal;
    if (len == 1) {
        pal = (uint64_t)(m + 1);       /* palindromes 1..9 */
    } else {
        /*  The first prefix has the form 10^(half-1).  */
        int half = (len + 1) / 2;
        uint64_t start = pow10[half - 1];
        uint64_t prefix = start + m;   /* the specific prefix for this index */

        if (len % 2 == 0) {           /* even length */
            pal = prefix * pow10[len / 2] + rev_uint64(prefix);
        } else {                      /* odd length */
            pal = prefix * pow10[(len - 1) / 2] + rev_uint64(prefix / 10);
        }
    }

    return exp_of_11(pal);
}
