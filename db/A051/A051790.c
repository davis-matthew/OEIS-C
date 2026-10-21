/*
 * A051790  a(n) = C(n)*(C(n)-1)*(C(n)-2)/6, where C(n) are the Catalan
 *          numbers (A000108).
 * https://oeis.org/A051790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n)*(C(n)-1)*(C(n)-2)/6, where C(n) is the n‑th Catalan
 *            number. For each n ≥ 0 this function returns the exact value if
 *            it fits in seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline seq_t catalan(idx_t n)
{
    /* C(n) = binom(2*n, n) / (n+1) */
    seq_t b = binomial((idx_t)(2 * n), n);
    return b / ((seq_t)n + 1);
}

seq_t a(idx_t n)
{
    seq_t c   = catalan(n);          /* Catalan number C(n) */
    if (c < (seq_t)3)                /* C(n) < 3 ⇒ a(n) = 0 */
        return 0;

    /* Compute C(n) choose 3 without overflow on 64‑bit seq_t.
       Since C(n)*(C(n)-1)*(C(n)-2) is divisible by 6, we divide
       a factor of 2 and a factor of 3 from the three consecutive
       numbers before multiplying.             */
    seq_t a1 = c;
    seq_t b1 = c - 1;
    seq_t c1 = c - 2;

    /* Extract a factor of 2 */
    if ((a1 & 1) == 0)
        a1 /= 2;
    else if ((b1 & 1) == 0)
        b1 /= 2;
    else
        c1 /= 2;

    /* Extract a factor of 3 */
    if ((a1 % 3) == 0)
        a1 /= 3;
    else if ((b1 % 3) == 0)
        b1 /= 3;
    else
        c1 /= 3;

    /* Multiply the remaining factors. The intermediate result
       never overflows a seq_t that can hold the final value. */
    seq_t prod = a1 * b1;
    prod = prod * c1;
    return prod;
}
