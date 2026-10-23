/*
 * A166660  Totally multiplicative sequence with a(p) = 2p+1 for prime p.
 * https://oeis.org/A166660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2p+1)^e (totally multiplicative with a(p) =
 *            2p+1)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* defined above offset is irrelevant */
        return 1;

    uint64_t num = n;       /* work on a 64‑bit copy for factorisation */
    seq_t result = 1;       /* accumulated product */

    /* Trial division factorisation; fast enough for n up to at least 1e4 */
    for (uint64_t p = 2; p <= num / p; ++p)
    {
        if (num % p != 0)
            continue;

        unsigned exp = 0;
        while (num % p == 0)      /* count exponent of this prime */
        {
            num /= p;
            ++exp;
        }

        seq_t base = (seq_t)p * 2 + 1;   /* 2p+1 */
        seq_t term = 1;
        for (unsigned i = 0; i < exp; ++i)   /* compute (2p+1)^exp */
            term *= base;

        result *= term;
    }

    /* If num > 1, it is a prime factor with exponent 1 */
    if (num > 1)
    {
        seq_t base = (seq_t)num * 2 + 1;   /* 2p+1 for the remaining prime */
        result *= base;
    }

    return result;
}
