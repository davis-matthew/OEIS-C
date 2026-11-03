/*
 * A218831  a(n) is the least r > 1 for which the interval (r*n, r*(n+1))
 *          contains no prime, or a(n)=0 if no such r exists.
 * https://oeis.org/A218831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer r>1 such that the open interval *
 *            (r*n, r*(n+1)) contains no prime; if no such r exists a(n)=0. *
 *            * The algorithm searches r from 2 upward, testing whether the
 *            interval * contains a prime by finding the first prime > r*n and
 *            checking if it * lies before r*(n+1). It stops...
 * Range:     u8 none, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9575 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Upper limit for r to search.  The known maximal value in the
       sequence up to 5·10^7 is 16, so 1000 is comfortably above that
       and keeps the search short while still covering all verified
       terms. */
    const idx_t R_MAX = 1000;

    for (idx_t r = 2; r <= R_MAX; ++r)
    {
        /* Compute the bounds of the interval (r*n, r*(n+1)) as seq_t
           to avoid overflow; all intermediate values fit in seq_t
           because r and n are small for the search range. */
        seq_t low  = (seq_t)r * (seq_t)n + 1;     /* first possible prime > r*n      */
        seq_t high = (seq_t)r * (seq_t)(n + 1);   /* upper bound of the interval      */

        /* Find the first prime greater than r*n. */
        seq_t p = low;
        while (!is_prime_big(p))
            ++p;

        /* If that prime is not inside the interval, r is the answer. */
        if (p >= high)
            return (seq_t)r;
    }

    /* If we reach here no r up to R_MAX satisfies the condition;
       by definition a(n) = 0. */
    return (seq_t)0;
}
