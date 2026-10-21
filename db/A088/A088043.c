/*
 * A088043  Number of partitions of n into parts which can be arranged to form
 *          a geometric progression (possibly with a common ratio of 1). (For
 *          every partition there exists a geometric progression in which this
 *          partition fits in as successive terms.).
 * https://oeis.org/A088043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) + #{(r,k): r≥2, k≥2 and (r^k–1)/(r–1) | n} (d(n) =
 *            number of positive divisors of n, accounting for the r = 1 case,
 *            i.e., partitions (a, a, …, a) of length k)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Number of partitions of n into parts that can be arranged to form a
   geometric progression (ratio may be 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* below offset – undefined */

    /* ---- divisor count d(n) (the r = 1 contributions) ---- */
    seq_t d = 1;                                 /* starts as 1 */
    idx_t  t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) { t /= p; ++exp; }
            d *= (seq_t)(exp + 1);
        }
    }
    if (t > 1) d *= 2;                          /* remaining prime factor */

    /* ---- contributions from r ≥ 2 ---- */
    seq_t N = (seq_t)n;                          /* use seq_t for arithmetic */
    for (idx_t r = 2; r <= n - 1; ++r) {
        seq_t power = (seq_t)r;                  /* r^1 */
        seq_t sum   = 1 + power;                /* k = 2, sum = 1 + r */

        while (sum <= N) {                      /* while S <= n */
            if (N % sum == 0) ++d;              /* S divides n */

            /* Next power r^(k+1); stop if it would overflow n */
            if (power > N / (seq_t)r) break;
            power *= r;
            if (sum > N - power) break;         /* would exceed n */
            sum += power;                       /* add new term to the sum */
        }
    }

    return d;
}
