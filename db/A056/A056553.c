/*
 * A056553  Smallest 4th-power divisible by n divided by largest 4th-power
 *          which divides n.
 * https://oeis.org/A056553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9756 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p^e‖n} (p^4 if e mod 4 ≠ 0 else 1)
 * This follows from a(p^e) = 1 when e is a multiple of 4,
 * otherwise a(p^e) = p^4.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* Factor n into distinct primes and their exponents */
    factor(n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] % 4 != 0) {
            seq_t p = (seq_t)primes[i];
            seq_t p2 = p * p;
            seq_t p4 = p2 * p2;
            res *= p4;
        }
    }
    return res;
}
