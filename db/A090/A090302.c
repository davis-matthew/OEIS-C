/*
 * A090302  Begin with n and consider numbers obtained by successively
 *          subtracting 0, 1, 2, 3, ...; a(n) = largest prime that arises in
 *          the process, i.e., largest prime of the form n - T(r), where T(r)
 *          is the r-th triangular number; or 0 if no such number exists.
 * https://oeis.org/A090302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum prime that equals n - r(r+1)/2 for some
 *            non‑negative integer r, or 0 if no such prime exists
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9798 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t best = 0;          /* best prime found so far (0 means none yet) */
    idx_t  triangular = 0;   /* current triangular number T(r) = r(r+1)/2 */

    /* iterate over r = 0, 1, 2, ... until T(r) exceeds n */
    for (idx_t r = 0; /* empty */; ++r) {
        if (triangular > n) break;                 /* no further candidates */

        seq_t cand = (seq_t)(n - triangular);       /* candidate n - T(r) */

        if (cand >= 2 && is_prime_big(cand)) {
            if (cand > best) best = cand;           /* keep the largest prime */
        }

        triangular += r + 1;                         /* next triangular number */
    }

    return best;
}
