/*
 * A021726  Decimal expansion of 1/722.
 * https://oeis.org/A021726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 722⌋ mod 10 (n ≥ 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  static cache of digits (1‑indexed)  */
    static seq_t *dig = NULL;       /* dig[0] unused, dig[1] = first decimal digit, …   */
    static size_t  cap = 0;         /* allocated array size (including index 0)     */
    static idx_t   len  = 0;         /* highest digit index computed so far         */
    static uint64_t rem  = 1;        /* remainder after computing len digits (10^len mod 722) */

    if (n == (idx_t)-1) /* defensive: avoid unsigned underflow on subtraction below */
        return (seq_t)0;

    /* target digit index (1‑based) needed for a(n) */
    idx_t target = n + 1;

    /* grow cache if needed */
    if (target > len)
    {
        size_t needed = target + 1; /* for index 0 */
        if (cap < needed)
        {
            size_t newcap = (cap > 0) ? cap * 2 : 16;
            while (newcap < needed)
                newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(dig, newcap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failed */
            dig = tmp;
            cap = newcap;
            if (cap > 0)
                dig[0] = 0;            /* unused slot, keep value 0 */
        }

        /* compute remaining digits */
        for (idx_t i = len + 1; i <= target; ++i)
        {
            rem *= 10ull;                     /* rem < 7220, stays in 64‑bit */
            seq_t digit = rem / 722ull;       /* 0‑9 */
            rem   %= 722ull;
            dig[i] = digit;
        }
        len = target;
    }

    return dig[target];
}
