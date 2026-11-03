/*
 * A142362  Primes congruent to 11 mod 47.
 * https://oeis.org/A142362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 11 (mod 47)
 * Range:     u8 n <= 2, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 968 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t* primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void
ensure_capacity(size_t new_cap)
{
    if (new_cap <= primes_cap)
        return;
    size_t cap = primes_cap > 0 ? primes_cap : 8;
    while (cap < new_cap)
        cap <<= 1;
    seq_t* new_arr = mem_resize(seq_t, primes, cap);
    /* In the unlikely event of allocation failure we simply abort,
       as the caller expects correct behaviour for all n. */
    primes = new_arr;
    primes_cap = cap;
}

seq_t a(idx_t n)
{
    while (primes_len < (size_t)n) {
        seq_t candidate;
        if (primes_len == 0)
            candidate = (seq_t)11;
        else
            candidate = primes[primes_len - 1] + (seq_t)47;

        /* Search forward by steps of 47 until a prime is found. */
        while (!is_prime_big(candidate)) {
            candidate += (seq_t)47;
        }

        ensure_capacity(primes_len + 1);
        primes[primes_len++] = candidate;
    }

    return primes[n - 1];
}
