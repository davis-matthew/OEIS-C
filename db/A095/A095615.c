/*
 * A095615  a(n) = 112 written in base 10 - n.
 * https://oeis.org/A095615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 112 written in base (10 - n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for sequence A095615.
   It works for all n ≥ 0. For bases outside 1..10 or |base|>10,
   the function returns 0. The base 1 case (unary) is only yielded
   for arbitrary-precision builds; for 64‑bit builds it returns 0. */
seq_t a(idx_t n)
{
    int64_t base = 10 - (int64_t)n;

    /* Handle the special base == 1 case (unary representation). */
    if (base == 1) {
        /* Unary representation: 112 consecutive '1' digits. */
        /* Only compute if seq_t is big enough. */
        if (sizeof(seq_t) > 8) {
            seq_t r = 0;
            for (idx_t i = 0; i < 112; ++i)
                r = r * 10 + 1;
            return r;
        } else {
            return 0;   /* 64‑bit builds cannot handle this term. */
        }
    }

    /* Base 0 or impossible bases. */
    if (base == 0)
        return 0;

    /* Positive base in [2,10] – standard base conversion. */
    if (base >= 2 && base <= 10) {
        int temp = 112;
        int digits[64];
        int len = 0;
        do {
            digits[len++] = temp % (int)base;
            temp /= (int)base;
        } while (temp > 0);
        seq_t result = 0;
        for (int i = len - 1; i >= 0; --i)
            result = result * 10 + (seq_t)digits[i];
        return result;
    }

    /* Negative base with absolute value ≤10 – conversion with adjustment. */
    if (base <= -2 && -(int)base <= 10) {
        int64_t temp = 112;
        int digits[64];
        int len = 0;
        while (temp != 0) {
            int64_t rem = temp % base;   /* base is negative */
            temp /= base;
            if (rem < 0) {
                rem -= base;            /* make remainder non‑negative */
                temp++;
            }
            digits[len++] = (int)rem;
        }
        seq_t result = 0;
        for (int i = len - 1; i >= 0; --i)
            result = result * 10 + (seq_t)digits[i];
        return result;
    }

    /* All other bases are not defined for this sequence. */
    return 0;
}
