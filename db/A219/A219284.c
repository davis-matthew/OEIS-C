/*
 * A219284  Primes of the form 17^n - 16^n.
 * https://oeis.org/A219284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = the n-th prime of the form 17^k - 16^k (in increasing
 *            order)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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

/* Static state for memoisation ------------------------------------------------ */
static seq_t *primes = NULL;   /* array of found primes  */
static size_t  primes_cap = 0; /* allocated capacity     */
static size_t  primes_len = 0; /* number of primes stored*/
static idx_t   next_exp = 0;    /* exponent for next check */
static seq_t   pow16 = 1, pow17 = 1; /* 16^e and 17^e for next_exp */

/* ----------------------------------------------------------------------------- */
/* Ensure that at least n primes have been found.                                */
static void ensure_terms(idx_t n)
{
    /* initialise array if necessary */
    if (primes == NULL) {
        grow_seq(&primes, &primes_cap, 16);
        primes_len = 0;
    }

    /* iterate exponents until we have enough primes */
    while (primes_len < (size_t)n) {
        /* advance exponent and update powers of 16 and 17 */
        next_exp++;
        pow16 = mul_small(pow16, 16);
        pow17 = mul_small(pow17, 17);

        /* compute difference 17^k - 16^k */
        seq_t diff = pow17;
        diff -= pow16;

        /* test primality */
        if (is_prime_big(diff)) {
            /* grow storage if needed */
            grow_seq(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = diff;
        }
    }
}

/* ----------------------------------------------------------------------------- */
/* Required exported function                                                     */
seq_t a(idx_t n)
{
    if (n == 0)          /* no element for a(0) by convention */
        return 0;
    ensure_terms(n);
    return primes[(size_t)n - 1];
}
