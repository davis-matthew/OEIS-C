/*
 * A066674  Least number m such that phi(m) = A000010(m) is divisible by the
 *          n-th prime.
 * https://oeis.org/A066674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime m such that m ≡ 1 (mod pₙ), where pₙ is the
 *            n‑th prime. In other words, a(n) is the smallest prime ≡ 1 (mod
 *            (n‑th prime))
 * Range:     u8 n <= 10, u16 n <= 283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9931 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

/* Cached prime list */
static uint64_t *s_primes = NULL;
static size_t    s_prime_count = 0;

/* Ensure that the cached list contains at least the first n primes. */
static void ensure_prime_limit(idx_t n)
{
    if (s_prime_count >= n) return;

    /* A crude upper bound for the nth prime:  n · 20 + 10
       suffices for n ≤ 10⁴ (the 10 000‑th prime is 104 729). */
    idx_t limit = n * 20 + 10;
    if (limit < 100) limit = 100;          /* safety for very small n */

    /* Rebuild the prime list.  Free the old data if present. */
    free(s_primes);
    s_primes = NULL;
    s_prime_count = 0;
    build_primes_upto(limit, &s_primes, &s_prime_count);
}

/* --- Public function required by the task --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 – undefined for n < 1 */

    ensure_prime_limit(n);               /* guarantee pₙ is available */

    uint64_t p = s_primes[n - 1];         /* the n‑th prime (1‑based) */
    seq_t   mod = (seq_t)p;               /* convert to seq_t for arithmetic */

    idx_t k = 1;                          /* multiplier in m = k·p + 1 */
    for (;;) {
        seq_t m = (seq_t)k * mod + 1;     /* candidate m */
        if (is_prime_big(m))
            return m;                    /* found the least prime ≡ 1 (mod pₙ) */
        ++k;
    }
}
