/*
 * A062172  Table T(n,k) by antidiagonals of n^(k-1) mod k [n,k > 0].
 * https://oeis.org/A062172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062172
 * Formula: a(n) = T(m,k) where (m,k) are the indices of the
 * antidiagonal enumeration of the table T(n,k)=n^(k-1) mod k.
 * For an index n (1‑based) we find the diagonal s such that
 * (s-1)*s/2 >= n and use the offset inside the diagonal to obtain
 * n = 1+offset, k = s-1-offset. Then a(n) = (n)^(k-1) mod k.
 */

#include <stdint.h>
#include <math.h>

/* modular exponentiation with 64‑bit intermediate to avoid overflow */
static inline seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (seq_t)((unsigned __int128)result * b % mod);
        b = (seq_t)((unsigned __int128)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* compute a(n) */
seq_t a(idx_t idx)
{
    /* idx >= 1 */
    const uint64_t n = idx;

    /* find diagonal s with (s-1)*s/2 >= n */
    uint64_t s = (uint64_t)((1 + sqrt((long double)(1 + 8.0L * (long double)n))) / 2);
    /* adjust to the correct minimal s */
    while ((unsigned __int128)s * (s - 1) / 2 < n) ++s;
    while (s > 1 && (unsigned __int128)(s - 1) * (s - 2) / 2 >= n) --s;

    /* previous diagonal total */
    uint64_t prev = (unsigned __int128)(s - 2) * (s - 1) / 2;
    uint64_t offset = n - prev - 1;      /* 0‑based inside diagonal */

    uint64_t m = 1 + offset;             /* row (original n) */
    uint64_t k = (s - 1) - offset;       /* column (original k) */

    if (k == 1) return 0;                /* n^0 mod 1 = 0 */

    return mod_pow_uint64(m % k, k - 1, k);
}
