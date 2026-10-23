/*
 * A106971  Primes of the form 5x^2+4xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   isPrime64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106971: Prime numbers of the form 5·x² + 4·x·y + 5·y² for
 * non‑negative integers x, y.  The function checks successive primes
 * and tests each one for representability by the given quadratic form. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrime64.h"
#include "../utils/isqrt.h"

/* Return true if a prime p can be written as 5x²+4xy+5y² with x,y ≥ 0.  */
static bool is_representable(seq_t p)
{
    /* We only need to try x until 5x² > p.  */
    for (idx_t x = 0; ; ++x) {
        seq_t x_seq = (seq_t)x;
        seq_t x2 = x_seq * x_seq;

        seq_t term5x2 = 5 * x2;
        if (term5x2 > p)          /* no further representations possible */
            break;

        /* Compute 20p - 84x² safely.  */
        seq_t lhs = 84 * x2;          /* 84x²  */
        seq_t rhs = 20 * p;           /* 20p   */
        if (rhs < lhs)                /* would underflow, skip this x */
            continue;

        seq_t delta = rhs - lhs;      /* discriminant 20p - 84x² */
        seq_t s = isqrt(delta);       /* integer square root */
        if (s * s != delta)           /* not a perfect square */
            continue;

        /* s must be at least 4x to make y non‑negative.  */
        seq_t four_x = 4 * x_seq;
        if (s < four_x)
            continue;

        seq_t diff = s - four_x;      /* 10y = s - 4x */
        if ((diff % (seq_t)10) != 0)   /* y not integral */
            continue;

        /* y = diff / 10 is automatically non‑negative.  Representation found. */
        return true;
    }
    return false; /* no representation found */
}

/* Return the nth prime (n≥1) that can be expressed as 5x²+4xy+5y².  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    idx_t count = 0;
    uint64_t p = 2;                /* start from the smallest prime */

    while (true) {
        if (isPrime64(p) && is_representable((seq_t)p)) {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
        /* Advance to the next candidate prime. */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }
}
