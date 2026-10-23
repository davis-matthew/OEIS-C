/*
 * A014188  Fourth powers of palindromes.
 * https://oeis.org/A014188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A014188 – Fourth powers of palindromes
   a(n) = (p_n)^4  where p_n is the n‑th decimal palindrome
   (counting from 0, with 0,1,2,...,9,11,22,33,...).
 */

#include <stdint.h>
#include <stdbool.h>

/* 10^exp for non‑negative small exp (exp <= 20) */
static inline uint64_t pow10_uint64(unsigned exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10;
    return r;
}

/* decimal reverse of a non‑negative integer */
static inline uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x) {
        rev = rev * 10 + x % 10;
        x   /= 10;
    }
    return rev;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the first 10 single‑digit palindromes directly. */
    if (n < 10) {
        seq_t v = (seq_t)n;
        seq_t sq = v * v;        /* square */
        return sq * sq;          /* fourth power */
    }

    /* Reduce index to the range starting from the first 2‑digit palindrome. */
    idx_t rem = n - 10;           /* remaining offset */

    unsigned len = 2;             /* current palindrome length */
    for (;;) {
        unsigned half = (len + 1) / 2;         /* number of first‑half digits */
        idx_t   cnt   = 9;                     /* 9 * 10^(half-1) palindromes of this length */

        for (unsigned i = 1; i < half; ++i) {
            /* cnt fits in 64‑bit for all n we expect to handle. */
            cnt *= 10;
        }

        if (rem < cnt) {
            /* Construct the palindrome of the given length and offset. */
            uint64_t start     = pow10_uint64(half - 1);   /* minimal first‑half (no leading 0) */
            uint64_t firsthalf = start + (uint64_t)rem;    /* first‑half value */

            uint64_t pal;
            if (len & 1) {   /* odd length: exclude middle digit when reflecting */
                pal = firsthalf * pow10_uint64(half - 1)
                    + reverse_uint64(firsthalf / 10);
            } else {         /* even length: reflect entire first‑half */
                pal = firsthalf * pow10_uint64(half)
                    + reverse_uint64(firsthalf);
            }

            /* Compute the fourth power. */
            seq_t v  = (seq_t)pal;
            seq_t sq = v * v;
            return sq * sq;
        }

        rem -= cnt;    /* move to the next length */
        ++len;
    }
}
