/*
 * A228031  Primes of the form 7^k + 10.
 * https://oeis.org/A228031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 7^k + 10 (OEIS A228031) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Utility headers - provided by the build environment */

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Static state to cache found primes and the current power of 7 */
    static seq_t *primes   = NULL;   /* array of primes of this form */
    static size_t  cap     = 0;      /* capacity of the array */
    static size_t  count   = 0;      /* number of primes stored so far */
    static seq_t  power    = 1;      /* 7^k, starting with k = 0 */

    /* Allocate initial cache */
    if (primes == NULL) {
        cap = 16;
        primes = mem_new(seq_t, cap);
        if (!primes) exit(EXIT_FAILURE);
    }

    /* Generate further primes until we have at least n of them */
    while (count < n) {
        seq_t val = power + (seq_t)10;          /* candidate 7^k + 10 */

        if (is_prime_big(val)) {               /* test primality */
            if (count == cap) {                /* grow cache if needed */
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, primes, cap);
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[count++] = val;
        }

        power = mul_small(power, 7);            /* advance to next 7^k */
    }

    return primes[n - 1];
}
