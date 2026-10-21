/*
 * A088644  Smallest n-digit number divisible by n!.
 * https://oeis.org/A088644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest n‑digit number divisible by n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (call n>=1; returns 0 if no such number exists) */

#include <stdint.h>   /* for std::uint64_t etc. */

seq_t a(idx_t n)
{
    /* n==0 is not part of the sequence; return 0 just in case */
    if (n == 0) return 0;

    /* The factorial grows very fast.
       For 64‑bit seq_t we can only represent up to 20! safely.
       For 128‑bit seq_t we can represent up to 25! safely.
       For larger n there is no n‑digit multiple of n! anyway.  */
    if (sizeof(seq_t) <= 8 && n > 20) return 0;
    if (n > 25) return 0;

    /* compute factorial f = n!   (always fits in 128 bits here) */
    unsigned __int128 f = 1;
    for (idx_t i = 2; i <= n; ++i) f *= i;

    /* compute 10^n   (fits in 128 bits for n<=25) */
    unsigned __int128 pow10_n = 1;
    for (idx_t i = 0; i < n; ++i) pow10_n *= 10;

    /* if n! has more than n digits, no n‑digit multiple exists */
    if (f >= pow10_n) return 0;

    /* compute 10^(n-1) */
    unsigned __int128 pow10_nm1 = pow10_n / 10;

    /* k = ceil(10^(n-1) / n!)  */
    unsigned __int128 k = (pow10_nm1 + f - 1) / f;

    /* result = k * n!  */
    unsigned __int128 val = k * f;

    return (seq_t)val;   /* will truncate on 64‑bit builds when val fits */
}
