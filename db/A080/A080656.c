/*
 * A080656  Order of group of n X n X n Rubik cube, under assumptions not-s,
 *          m, not-i.
 * https://oeis.org/A080656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080656: Order of group of n×n×n Rubik cube (no twist/marking/invisible assumptions)
   Formula: a(1)=1; a(2)=7!*3^6; a(3)=8!*3^7*12!*2^10;
            a(n)=a(n-2)*24!*(24!/2)^(n-3)  for n>=4
   The recurrence skips one index, so we keep the last even‑index term and the last odd‑index term.
   The function works for any seq_t type that supports the usual arithmetic operators. */
#include <stdint.h>

/* fast exponentiation: base^(exp) where exp fits into uint64_t */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    uint64_t e = exp;
    while (e)
    {
        if (e & 1ULL)
            result *= b;
        e >>= 1ULL;
        if (e)
            b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* initial terms – constants that fit into seq_t when possible */
    static seq_t init = 0;
    static seq_t a2, a3;         /* a(2) and a(3) */
    static seq_t C1, C2;         /* 24! and 24!/2 */
    static seq_t even_last, odd_last;   /* a(n-2) for the current parity */

    if (!init)
    {
        init = (seq_t)1;

        /* compute 7! * 3^6 */
        a2 = (seq_t)1;
        for (idx_t i = 2; i <= 7; ++i) a2 *= (seq_t)i;
        a2 *= (seq_t)729;           /* 3^6 */

        /* compute 8! * 3^7 * 12! * 2^10 */
        a3 = (seq_t)1;
        for (idx_t i = 2; i <= 8; ++i) a3 *= (seq_t)i;
        a3 *= (seq_t)2187;           /* 3^7 */
        for (idx_t i = 2; i <= 12; ++i) a3 *= (seq_t)i;
        a3 *= (seq_t)1024;           /* 2^10 */

        /* compute 24! and 24!/2 */
        C1 = (seq_t)1;
        for (idx_t i = 2; i <= 24; ++i) C1 *= (seq_t)i;
        C2 = C1 / (seq_t)2;

        /* store the last terms for each parity */
        even_last = a2;            /* a(2) as last even term */
        odd_last  = a3;            /* a(3) as last odd term */
    }

    if (n == 1) return (seq_t)1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* iterate from 4 up to n, updating only the relevant parity */
    for (idx_t i = 4; i <= n; ++i)
    {
        uint64_t pow_exp = i - 3;            /* exponent for (24!/2) */
        seq_t factor = C1 * pow_seq_t(C2, pow_exp);

        if ((i & 1ULL) == 0ULL)        /* even index */
        {
            even_last *= factor;
        }
        else                          /* odd index */
        {
            odd_last *= factor;
        }
    }

    return (n & 1ULL) ? odd_last : even_last;
}
