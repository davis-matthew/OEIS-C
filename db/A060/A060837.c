/*
 * A060837  List the positive rationals in the canonical order
 *          A020652(n)/A020653(n) and apply the Sagher map to turn them into
 *          integers.
 * https://oeis.org/A060837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 463, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating reduced positive rationals in canonical
   order (numerator+denominator ascending, then numerator ascending),
   then applying the Sagher map:
   a(n) = (A020652(n))^2 * ∏_{p^e‖A020653(n)} p^(2*e-1). */

#include "../utils/factor_u64.h"

static inline idx_t gcd64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */

    idx_t remaining = n;
    idx_t s = 2;                   /* current diagonal (sum of terms) */

    /* Find the diagonal containing the nth rational */
    while (1) {
        idx_t coprime_cnt = 0;
        for (idx_t i = 1; i < s; ++i)
            if (gcd64(i, s) == 1)
                ++coprime_cnt;

        if (remaining > coprime_cnt) {
            remaining -= coprime_cnt;
            ++s;
            continue;
        }

        /* Now the nth rational is on diagonal s at position 'remaining' */
        idx_t pos = 0;
        idx_t numerator = 0;
        for (idx_t i = 1; i < s; ++i) {
            if (gcd64(i, s) == 1) {
                ++pos;
                if (pos == remaining) {
                    numerator = i;
                    break;
                }
            }
        }
        idx_t denominator = s - numerator;

        /* Apply Sagher map: numerator squared */
        seq_t result = (seq_t)numerator * (seq_t)numerator;

        /* Multiply by denominator's prime factors raised to (2*exp-1) */
        if (denominator > 1) {
            size_t cnt = 0;
            uint64_t pr[64], ex[64];
            factor_u64((uint64_t)denominator, pr, ex, &cnt);
            for (size_t j = 0; j < cnt; ++j) {
                seq_t pow = 1;
                for (idx_t r = 0; r < (idx_t)(2 * ex[j] - 1); ++r)
                    pow *= (seq_t)pr[j];
                result *= pow;
            }
        }

        return result;
    }
}
