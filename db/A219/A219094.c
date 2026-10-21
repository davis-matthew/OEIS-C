/*
 * A219094  n/(b*c), where b/c is the least reduced fraction > 1 using
 *          divisors b and c of n.
 * https://oeis.org/A219094
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/(b*c) where b/c is the least reduced fraction >1 made
 *            from coprime divisors b and c of n (b > c)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Euclidean algorithm for idx_t */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Offset is 2 – return 0 for values below the offset */
    if (n < 2)
        return 0;

    /* Collect all divisors of n.  For n up to 2^64 the number of
       divisors is < 103680, a conservative stack buffer of 4096 is
       ample for the intended limits. */
    idx_t divs[4096];
    size_t dcnt = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[dcnt++] = d;
            if (d * d != n) {
                divs[dcnt++] = n / d;
                if (dcnt >= sizeof(divs) / sizeof(divs[0]))
                    break; /* safety – should never happen for n within limits */
            }
        }
    }

    /* Find the coprime pair (b,c) with b>c and minimal b/c > 1. */
    idx_t best_b = 0; /* sentinel */
    idx_t best_c = 1;

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t b = divs[i];
        for (size_t j = 0; j < dcnt; ++j) {
            idx_t c = divs[j];
            if (b <= c)
                continue;
            if (gcd_u64(b, c) != 1)
                continue;

            if (best_b == 0) {
                best_b = b;
                best_c = c;
                continue;
            }

            /* compare b/c < best_b/best_c  using cross‐multiplication */
            seq_t left  = (seq_t)b * (seq_t)best_c;
            seq_t right = (seq_t)best_b * (seq_t)c;
            if (left < right) {
                best_b = b;
                best_c = c;
            }
        }
    }

    /* best_b,best_c are guaranteed to exist for n>=2. */
    idx_t bc = best_b * best_c;          /* bc divides n */
    return ((seq_t)n) / ((seq_t)bc);
}
