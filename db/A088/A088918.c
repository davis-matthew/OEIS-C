/*
 * A088918  Number of representations of n as sum of two squares of distinct
 *          primes.
 * https://oeis.org/A088918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of distinct primes (p < q) such
 *            that p^2 + q^2 = n. This implementation uses trial division to
 *            test primality of numbers up to √n and an integer square‑root
 *            routine based on binary search with 128‑bit intermediate
 *            multiplications for safety
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
/* Ensure the types seq_t and idx_t are defined by the build system. */

/* Integer square root for unsigned 64‑bit values.  Uses binary search
   with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = v;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Simple primality test sufficient for numbers up to √n.
   Uses trial division by odd numbers up to √x. */
static inline bool is_prime_small(idx_t x)
{
    if (x < 2)             return false;
    if (x % 2 == 0)        return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)    return false;
    return true;
}

/* Main sequence function.  It works for any n >= 1 and is independent
   of call order. */
seq_t a(idx_t n)
{
    if (n < 13)          /* 2^2 + 3^2 = 13 is the smallest possible sum */
        return 0;

    seq_t count = 0;
    idx_t p_max = isqrt_u64(n - 1);   /* largest possible p with p^2 < n */

    for (idx_t p = 2; p <= p_max; ++p) {
        if (!is_prime_small(p))
            continue;

        idx_t p2 = p * p;
        idx_t rem = n - p2;
        if (rem <= p2)          /* ensures q > p */
            break;

        idx_t q = isqrt_u64(rem);
        if (q * q != rem)    /* not a perfect square */
            continue;
        if (q <= p)          /* need distinct; p < q */
            continue;
        if (!is_prime_small(q))
            continue;

        ++count;
    }

    return count;
}
