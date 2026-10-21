/*
 * A060265  Largest prime less than 2n.
 * https://oeis.org/A060265
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A060265 – the largest prime less than 2*n.
 * Formula:  a(n) = max{ p prime | p < 2n }  (for n ≥ 2)
 */

#include <stdint.h>
#include <stdbool.h>

/* 64‑bit deterministic Miller–Rabin primality test (works up to 2^64). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2)          return false;
    if (n < 4)          return true;     // 2,3
    if ((n & 1) == 0)   return false;    // even

    /* write n-1 as d*2^s  with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases for deterministic test up to 2^64 */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (int i = 0; i < (int)(sizeof(bases)/sizeof(bases[0])); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) return true;    /* a is multiple of n */

        /* compute a^d mod n */
        __uint128_t x = 1, p = a % n;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*  Sequence function */
seq_t a(idx_t n)
{
    /* Offset is 2; for n < 2 the value is undefined – return 0 */
    if (n < 2)
        return 0;

    /* limit = 2*n  (fits in seq_t for all reasonably small n) */
    seq_t limit = (seq_t)n * 2ULL;

    /* search downward from (2*n)-1 for the first prime */
    for (seq_t v = limit - 1; v >= 2; --v) {
        if (is_prime_uint64((uint64_t)v))
            return v;
    }
    return 0; /* unreachable for n ≥ 2 */
}
