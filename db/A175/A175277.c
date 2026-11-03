/*
 * A175277  Base-5 pandigital primes: primes having at least one of each digit
 *          0,1,2,3,4, when written in base 5.
 * https://oeis.org/A175277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime that is base‑5 pandigital * (contains each
 *            of the digits 0,1,2,3,4 at least once when written * in base 5).
 *            The implementation searches primes sequentially, * stores the
 *            ones that satisfy the pandigital test and returns the *
 *            requested element. It is memoised so that calls w...
 * Range:     u8 none, u16 n <= 1080, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9783 (2026-11-03)
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

/* helper: test for base‑5 pandigital (at least 6 base‑5 digits) */
static bool is_base5_pandigital(seq_t x)
{
    bool seen[5] = {false, false, false, false, false};
    int digits = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 5);
        seen[d] = true;
        t /= 5;
        digits++;
    }
    return (digits >= 6 &&
            seen[0] && seen[1] && seen[2] && seen[3] && seen[4]);
}

/* static storage for memoisation */
static seq_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static seq_t next_candidate = 3125ULL; /* 5^5 */

/* grow the primes array if needed */
static void ensure_capacity(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    while (need > new_cap)
        new_cap *= 2;
    seq_t *new_arr = mem_resize(seq_t, primes, new_cap);
    if (!new_arr)
        exit(1); /* allocation failure */
    primes = new_arr;
    primes_cap = new_cap;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* behaviour below offset does not matter */

    /* initialise (only on first call) */
    if (primes == NULL) {
        primes = NULL;
        primes_len = 0;
        primes_cap = 0;
        /* ensure odd start for all subsequent candidates */
        if ((next_candidate & 1) == 0)
            next_candidate++;
    }

    /* generate primes until we have at least n elements */
    while (primes_len < n) {
        /* find next prime that is base‑5 pandigital */
        while (true) {
            if (is_prime_big(next_candidate) && is_base5_pandigital(next_candidate)) {
                break; /* found one */
            }
            next_candidate += 2; /* skip even numbers */
        }

        /* store found prime */
        ensure_capacity(primes_len + 1);
        primes[primes_len++] = next_candidate;
        next_candidate += 2; /* advance for next search */
    }

    return primes[n - 1];
}
