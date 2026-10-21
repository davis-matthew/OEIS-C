/*
 * A075801  Differences between adjacent palindromic nonprime numbers A032350.
 * https://oeis.org/A075801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = P(n+1) – P(n) where P(k) is the k‑th palindromic
 *            non‑prime (including 1). The function scans palindromes in
 *            ascending order, skips the primes, and returns the difference
 *            after collecting the (n+1)‑th non‑prime palindrome
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- deterministic Miller–Rabin for 64‑bit unsigned integers */
static bool is_prime(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    if (n % 5 == 0)     return n == 5;
    if (n % 7 == 0)     return n == 7;
    if (n % 11 == 0)    return n == 11;
    if (n % 13 == 0)    return n == 13;
    if (n % 17 == 0)    return n == 17;

    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    const size_t nb = sizeof(bases)/sizeof(bases[0]);

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < nb; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, mod = n, e = d;

        /* modular exponentiation a^d mod n */
        while (e) {
            if (e & 1) x = (x * p) % mod;
            p = (p * p) % mod;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- build a palindrome from its half (most significant half) --
   odd == true  → odd length palindrome, skip middle digit when mirroring */
static uint64_t make_pal(uint64_t half, bool odd)
{
    uint64_t pal = half;
    if (odd) half /= 10;                    /* drop middle digit for odd */
    while (half > 0) {
        pal = pal * 10 + (half % 10);
        half /= 10;
    }
    return pal;
}

/* --- main sequence implementation --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined but harmless */

    const idx_t target = n + 1;             /* need the (n+1)-th palindrome */
    idx_t   seen = 0;                       /* count of non‑prime palindromes seen */
    uint64_t prev = 0;                      /* previous non‑prime palindrome */

    for (int len = 1; ; ++len) {            /* iterate over palindrome lengths */
        const bool odd = (len & 1);
        const int halfLen = (len + 1) / 2;

        uint64_t start = 1;
        for (int i = 1; i < halfLen; ++i) start *= 10;   /* 10^(halfLen-1) */
        uint64_t end   = start * 10;                    /* 10^halfLen     */

        for (uint64_t half = start; half < end; ++half) {
            const uint64_t pal = make_pal(half, odd);
            if (!is_prime(pal)) {               /* palindromic non‑prime */
                ++seen;
                if (seen == 1) {
                    prev = pal;
                } else {
                    if (seen == target) {        /* nth difference found */
                        return (seq_t)(pal - prev);
                    }
                    prev = pal;                  /* update for next step */
                }
            }
        }
    }
}
