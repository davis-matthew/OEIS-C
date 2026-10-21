/*
 * A067121  a(n) = floor[X/Y] where X = the concatenation of the first n even
 *          numbers in increasing order and Y = their sum.
 * https://oeis.org/A067121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor(concatenation of 2,4,…,2n / (2+4+…+2n))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by iteratively maintaining the quotient q and the remainder r
   of the partial concatenation divided by Y = n*(n+1).  This avoids storing a huge
   concatenated integer and works for any build of seq_t (fixed‑size or arbitrary
   precision). */
#include <stdint.h>

static inline int digit_count_uint64(uint64_t x) {
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

static const uint64_t POW10[6] = {1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL};

seq_t a(idx_t n) {
    /* Y = sum of the first n even numbers = n*(n+1) */
    unsigned __int128 Y = (unsigned __int128)n * (n + 1);

    seq_t q = 0;          /* quotient so far */
    unsigned __int128 r = 0;  /* remainder so far, always < Y */

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t ev = (uint64_t)(2 * k);          /* current even number */
        int d = digit_count_uint64(ev);          /* number of decimal digits */
        uint64_t pow = POW10[d];                 /* 10^d */

        /* temp = r*pow + ev */
        unsigned __int128 temp = r * pow + ev;

        uint64_t addq = (uint64_t)(temp / Y);    /* additional quotient part */
        r = temp % Y;                            /* new remainder */

        q = q * (seq_t)pow + (seq_t)addq;        /* update quotient */
    }

    return q;
}
