/*
 * A087991  Number of non-palindromic divisors of n.
 * https://oeis.org/A087991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = number of non-palindromic divisors of n
 * Range:     u8 n <= 1556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_palindrome.h"

/* Recursively generate divisors from prime factors, counting the
 * non‑palindromic ones.  The base is fixed to decimal (10). */
static void gen_nonpal(int idx, int total,
                       const uint64_t *primes, const unsigned *exps,
                       seq_t cur, seq_t *cnt)
{
    if (idx == total) {
        if (!is_palindrome(cur, 10))
            (*cnt)++;
        return;
    }
    seq_t val = cur;
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        gen_nonpal(idx + 1, total, primes, exps, val, cnt);
        val *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* n = 1 has only the divisor 1 which is a palindrome */
    if (n == 1)
        return 0;

    /* Factor n */
    uint64_t pf[64];       /* prime factors */
    uint64_t texp[64];     /* exponents as uint64_t */
    size_t cnt;
    factor_u64((uint64_t)n, pf, texp, &cnt);

    /* Convert exponent types for use in recursion */
    unsigned exps[64];
    for (size_t i = 0; i < cnt; ++i)
        exps[i] = (unsigned)texp[i];

    seq_t nonpal = 0;
    gen_nonpal(0, (int)cnt, pf, exps, (seq_t)1, &nonpal);
    return nonpal;
}
