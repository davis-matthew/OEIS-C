/*
 * A214988  Beatty sequence for sqrt(r), where r = (1+sqrt(5))/2 = golden
 *          ratio; complement of A214989.
 * https://oeis.org/A214988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt((1+sqrt(5))/2) * n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Scaling factor for integer approximation of t = sqrt((1+sqrt(5))/2). */
static const idx_t B = ((idx_t)1 << 20);   /* 2^20 */

/* Cache for the integer numerator t_num = floor(t * B). */
static bool t_num_init = false;
static seq_t t_num;

/* Test whether S <= floor(t * B), equivalently S^2 <= B^2 * r
   where r = (1 + sqrt(5)) / 2. */
static inline bool leq_mid(idx_t S)
{
    __int128 S2   = (__int128)S * S;          /* S^2                 */
    __int128 B2   = (__int128)B * B;          /* B^2                 */
    __int128 lhs  = 2 * S2 - B2;              /* lhs = 2*S^2 - B^2   */
    if (lhs <= 0)  /* then lhs <= sqrt(5)*B^2  */
        return true;

    /* lhs > 0 : compare squares to avoid sqrt(5) */
    __int128 lhs2 = lhs * lhs;                /* (2*S^2 - B^2)^2     */
    __int128 rhs2 = (__int128)5 * B2 * B2;    /* 5 * B^4             */
    return lhs2 <= rhs2;
}

/* Compute t_num once, using binary search. */
static void init_t_num(void)
{
    idx_t lo = 0;
    idx_t hi = B << 1;            /* Upper bound : 2*B is safe */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (leq_mid(mid))
            lo = mid;
        else
            hi = mid - 1;
    }
    t_num = lo;
    t_num_init = true;
}

/* Beatty sequence a(n) = floor(t * n) */
seq_t a(idx_t n)
{
    if (!t_num_init)
        init_t_num();

    /* numerator: (t_num * n) */
    __uint128_t prod = (__uint128_t)t_num * (__uint128_t)n;

    /* denominator: B */
    return (seq_t)(prod / ( __uint128_t )B);
}
