/*
 * A063043  Collatz-2 (A063041) trajectory starting at 17.
 * https://oeis.org/A063043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   smallest_prime_factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A063041 (Collatz‑2 trajectory starting at 17)
 * a(0) = 17
 * a(n) = A063041(a(n-1))            for n > 0
 *
 * Transformation:
 *   even  -> divide by 2
 *   odd
 *      prime  -> multiply by the largest prime < x, add 1
 *      composite (odd) -> divide by its smallest prime factor
 *
 * The sequence eventually falls into the cycle 9 → 3 → 7 → 36 → 18 → 9,
 * so all values are guaranteed to fit in seq_t for the tested ranges.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor_uint64.h"

/* ---------- 64‑bit helper functions ---------------------------------- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof *small; ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof *bases; ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        /* modular exponentiation */
        uint64_t x = 1, base = a % n;
        uint64_t e = d;
        while (e)
        {
            if (e & 1) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static inline uint64_t largest_prime_less_uint64(uint64_t n)
{
    if (n <= 2) return 0;          /* no prime < 2 */
    for (uint64_t x = n - 1; x >= 2; --x)
        if (is_prime_uint64(x)) return x;
    return 0;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                        */
seq_t a(idx_t n)
{
    seq_t x = 17;
    if (n == 0) return x;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* even case: divide by 2 */
        if ((x & 1) == 0)
        {
            x = x / 2;
            continue;
        }

        /* odd case */
        uint64_t uv = (uint64_t)x;  /* safe: sequence values stay small */

        if (is_prime_uint64(uv))
        {
            uint64_t p = largest_prime_less_uint64(uv);
            x = x * (seq_t)p + 1;
        }
        else
        {
            uint64_t spf = smallest_prime_factor_uint64(uv);
            x = x / (seq_t)spf;
        }
    }
    return x;
}
