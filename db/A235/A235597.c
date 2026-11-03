/*
 * A235597  Squares in which each digit exceeds the previous digit by more
 *          than one.
 * https://oeis.org/A235597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235597: Squares in which each digit exceeds the previous digit by more than one. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return true if the decimal digits of x satisfy
   each later digit > previous digit + 1. */
static bool is_good_sq(seq_t x)
{
    if (x == (seq_t)0)
        return true;                    /* 0 is a square and satisfies the condition */

    seq_t tmp = x;
    uint8_t digs[20];
    size_t d = 0;

    /* Extract digits in least‑to‑most order. */
    while (tmp > (seq_t)0) {
        digs[d++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Check the condition from most‑to‑least significant digit. */
    for (size_t j = d - 1; j > 0; --j) {
        if (digs[j - 1] <= digs[j] + 1)
            return false;
    }
    return true;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    static seq_t terms[20];
    static size_t count = 0;

    if (count == 0) {
        /* Precompute all terms.  
           All qualifying squares are less than 100000 (=10^5). */
        const seq_t LIMIT = 100000 - 1;     /* largest 5‑digit number */
        for (seq_t m = 0;; ++m) {
            seq_t sq = (seq_t)m * (seq_t)m;
            if (sq > LIMIT)
                break;
            if (is_good_sq(sq))
                terms[count++] = sq;
        }
    }

    if (n == 0 || (size_t)(n - 1) >= count)
        return (seq_t)0;                  /* out of bounds → 0 */

    return terms[n - 1];
}
