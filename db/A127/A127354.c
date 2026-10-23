/*
 * A127354  Digit count of prime(n). The digit count numerically summarizes
 *          the frequency of digits 0 through 9 in that order when they occur
 *          in a number.
 * https://oeis.org/A127354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit count of the n‑th prime, i.e. A047842(A000040(n))
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 9802, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so caller should use n >= 1. */
    if (n == 0)
        return 0;                       /* undefined, return 0 as a safe default */

    /* Fetch the n‑th prime (1‑based). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Count the decimal digits 0–9 in the prime. */
    unsigned char cnt[10] = {0};
    digit_counts(p, cnt);

    /* Build the encoded result by concatenating count‑digit pairs. */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        int c = cnt[d];
        if (c == 0)
            continue;                      /* skip digits that do not appear */

        seq_t pair = ((seq_t)c) * 10 + d;   /* numeric value of count–digit pair */
        int dl   = digit_len(pair);        /* number of decimal digits in pair   */
        seq_t mul = pow10((idx_t)dl);      /* multiplier to shift the current result */

        result = result * mul + pair;
    }

    return result;
}
