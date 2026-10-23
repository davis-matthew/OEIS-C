/*
 * A132656  Motzkin number M(n) raised to power M(n).
 * https://oeis.org/A132656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = M(n)^M(n) where M(n) is the Motzkin number. M(0)=1,
 *            M(1)=1, M(n)=((2n+1)M(n-1)+3(n-1)M(n-2))/(n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Static cache for Motzkin numbers. */
    static seq_t *motz = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* largest index computed so far */

    if (n == 0 || n == 1) {
        /* Motzkin numbers for 0 and 1 are 1. */
        static seq_t one = (seq_t)1;
        return one;
    }

    /* Ensure the cache is large enough. */
    ensure_cache(n, &motz, &cap);

    /* Initialize the first two terms if not already done. */
    if (last < 1) {
        motz[0] = (seq_t)1;
        motz[1] = (seq_t)1;
        last = 1;
    }

    /* Compute Motzkin numbers up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t m_prev2 = motz[i - 2];
        seq_t m_prev  = motz[i - 1];

        /* Numerator: (2i+1)*M(i-1) + 3(i-1)*M(i-2) */
        seq_t num1 = (((seq_t)2 * (seq_t)i) + (seq_t)1) * m_prev;
        seq_t num2 = ((seq_t)3 * ((seq_t)i - (seq_t)1)) * m_prev2;
        seq_t sum  = num1 + num2;

        /* Denominator: i+2 */
        seq_t denom = (seq_t)(i + 2);

        /* Division yields the Motzkin number. */
        seq_t m_curr = sum / denom;
        motz[i] = m_curr;
    }
    last = n;

    seq_t M_n = motz[n];      /* Motzkin number at index n */
    seq_t result = big_pow(M_n, M_n); /* compute M(n)^M(n) */

    return result;
}
