/*
 * A080684  Number of 13-smooth numbers <= n.
 * https://oeis.org/A080684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of positive integers <= n with all prime factors
 *            <= 13 (including 1). The function enumerates every 13‑smooth
 *            number by back‑tracking over the primes {2,3,5,7,11,13} and
 *            counts them
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 971 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recursive generation of 13‑smooth numbers. */
static void smooth_rec(const seq_t target, seq_t cur, int idx,
                       seq_t *cnt)
{
    /* Primes that compose 13‑smooth numbers.  For C++ with an
       arbitrary‑precision seq_t this initializer works because
       the type has a constructor from integer literals. */
    static const seq_t primes[6] = { 2, 3, 5, 7, 11, 13 };
    if (idx == 6) {                /* all primes processed – a valid number */
        (*cnt)++;                  /* count it (1 is included) */
        return;
    }
    seq_t p = primes[idx];
    seq_t val = cur;
    while (1) {
        smooth_rec(target, val, idx + 1, cnt);   /* use current product */

        /* Stop if multiplying by p would exceed target.
           Division is performed before multiplication to avoid overflow
           on primitive types; for big integer types the operators
           behave correctly. */
        if (target / p < val)
            break;
        val *= p;
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 0 – harmless for offset 1 */
        return 0;
    seq_t target = (seq_t)n;
    seq_t count  = 0;
    smooth_rec(target, (seq_t)1, 0, &count);
    return count;
}
