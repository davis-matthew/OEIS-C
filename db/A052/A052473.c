/*
 * A052473  a(n) = binomial(2*n-5,n-2) + 2.
 * https://oeis.org/A052473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-5, n-2) + 2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052473, offset 0 */

#include <stddef.h>
#include <stdint.h>

/* helper: multiplicative binomial for all seq_t types */
static seq_t binom_mult(idx_t top, idx_t bot)
{
    /* choose the smaller of k and n-k */
    if (bot > top - bot)
        bot = top - bot;

    seq_t res = 1;
    for (idx_t i = 1; i <= bot; ++i)
    {
        seq_t num = (seq_t)(top - bot + i);

#ifdef __cplusplus
        /* in C++ seq_t is the arbitrary‑precision type; use normal ops */
        res = res * num / i;
#else
#   if defined(__SIZEOF_INT128__)
        /* use 128‑bit intermediate when available */
        __int128 temp = (__int128)res * (__int128)num;
        res = (seq_t)(temp / i);
#   else
        /* fallback (promoted to at least 32‑bit) */
        res = (res * num) / i;
#   endif
#endif
    }
    return res;
}

seq_t a(idx_t n)
{
    /* handle the first three indices explicitly */
    if (n == 0 || n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)3;

    /* n >= 3 */
    idx_t top = 2 * n - 5;
    idx_t bot = n - 2;

    seq_t bin = binom_mult(top, bot);
    return bin + (seq_t)2;
}
