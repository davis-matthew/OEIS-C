/*
 * A128492  Denominator of Sum_{k=1..n} 1/(2*k-1)^2.
 * https://oeis.org/A128492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the sum_{k=1..n} 1/(2*k-1)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"

/* GCD for unsigned __int128 ------------------------------------------- */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Small‐type implementation (seq_t <= 64 bits) ------------------------ */
static seq_t a_small(idx_t n)
{
    unsigned __int128 num = 0;      /* current numerator   */
    unsigned __int128 den = 1;      /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long odd = (unsigned long long)(2 * k - 1);
        unsigned long long d   = odd * odd;          /* d = (2k-1)^2 */

        unsigned __int128 new_num = num * d + den;
        unsigned __int128 new_den = den * d;

        unsigned __int128 g = gcd128(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }

    return (seq_t)den;   /* fits into seq_t (<= 64 bits) */
}

/* Large‐type implementation (seq_t > 64 bits) ------------------------ */
static seq_t a_big(idx_t n)
{
    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t odd = (seq_t)(2 * k - 1);
        seq_t d   = odd * odd;                     /* d = (2k-1)^2 */

        seq_t new_num = seq_mul(num, d) + den;
        seq_t new_den = seq_mul(den, d);

        seq_t g = seq_gcd(new_num, new_den);
        new_num = new_num / g;
        new_den = new_den / g;

        num = new_num;
        den = new_den;
    }

    return den;
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behavior for n==0 is irrelevant */

    /* Choose implementation according to the size of seq_t. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return a_small(n);
    else
        return a_big(n);
}
