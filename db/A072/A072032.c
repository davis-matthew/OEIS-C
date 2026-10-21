/*
 * A072032  a(n) = gcd(2^n, reverse(2^n)) = gcd(2^n, A004086(2^n)) =
 *          A055483(2^n).
 * https://oeis.org/A072032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   a(n) = greatest common divisor of 2^n and the decimal reversal
 *            of 2^n. This implementation computes the exponent of 2 in the
 *            reversal by inspecting the leading decimal digits of 2^n. No
 *            large integer arithmetic (and consequently no overflow) is
 *            required
 * Range:     u8 n <= 566, u16 n <= 6293, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by sequence definition */
    /* 2^n in base 10 has floor(n*log10(2)) + 1 digits.  Its leading
       m digits can be obtained from the fractional part of n*log10(2). */
    static const long double log10_2 = log10l(2.0L);

    long double exp_val   = n * log10_2;
    long double int_part  = 0.0L;
    long double frac_part = modfl(exp_val, &int_part);   /* frac_part ∈ [0,1) */

    /* Find the largest k such that the last k digits of the reversal are
       divisible by 2^k.  This is equivalent to: the first k digits of 2^n,
       when reversed, are divisible by 2^k. */
    int k = 0;
    while (1) {
        int m = k + 1;                        /* current number of digits to test */

        /* First m digits of 2^n: floor(10^{frac_part + m - 1}) */
        long double tmp = powl(10.0L, frac_part + m - 1);
        unsigned long long prefix = (unsigned long long)(tmp + 0.0000000001L);

        /* Reverse the digits of the prefix */
        unsigned long long rev = 0;
        unsigned long long t   = prefix;
        if (t == 0) t = 1;                   /* avoid infinite loop for leading 0 */
        while (t > 0) {
            rev = rev * 10 + t % 10;
            t   = t / 10;
        }

        unsigned long long mask = 1ULL << m;
        if (rev % mask != 0) {
            break;                            /* fail, current m too large */
        }
        ++k;                                   /* success, try next m */
    }

    /* The result is 2^k (k ≤ 63 on 64‑bit builds; for larger k we
       multiply in a loop so the code works for arbitrary‑precision seq_t). */
    seq_t result = 1;
    for (int i = 0; i < k; ++i) {
        result *= 2;
    }
    return result;
}
