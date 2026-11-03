/*
 * A158708  Primes p such that p + floor(p/2) is prime.
 * https://oeis.org/A158708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p + floor(p/2) is prime
 * Range:     u8 n <= 10, u16 n <= 729, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* helpers provided by the test harness */
#include "../utils/next_prime.h"
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
    if (n == 0)
        return 0; /* undefined for index 0, return 0 as a sentinel */

    idx_t count = 0;          /* how many terms we have found */
    uint64_t candidate = 1;   /* start before the first prime (2) */

    while (count < n) {
        /* get the next prime number */
        candidate = next_prime(candidate);

        /* compute q = p + floor(p/2). For p odd, floor(p/2) = (p-1)/2.
           For p=2, floor(p/2)=1, so q = 3. The formula (3*p-1)/2 works for
           all primes (including 2) because  (3*2-1)/2 = 5, which is the
           correct q value to test for primality. */
        uint64_t q = (3ULL * candidate - 1ULL) / 2ULL;

        if (is_prime_big((seq_t)q)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
    }

    /* unreachable, but keeps the compiler happy */
    return 0;
}
