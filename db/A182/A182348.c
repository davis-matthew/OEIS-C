/*
 * A182348  Primes of the form n^4 - 3.
 * https://oeis.org/A182348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A182348 – primes of the form n⁴ – 3
 *
 * The sequence lists primes p that can be written as n⁴ – 3 for some integer n ≥ 2.
 * a(1) = 13 (2⁴ – 3), a(2) = 4093 (8⁴ – 3), ...
 *
 * This implementation searches n in increasing order, tests the primality of
 * n⁴ – 3 with the supplied big-prime routine, and caches the primes
 * found so far.  The cache is resizable and independent of the order of
 * calls, so a(5) can be requested before a(3) without recomputation.
 *
 * The function uses `big_pow` for generic integer exponentiation so that
 * it works even when `seq_t` is an arbitrary‑precision type.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/ipow.h"
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

/* Resize the dynamic array to hold at least `need` elements. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **arr) {
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        if (newcap < need) newcap = need;
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
        if (!tmp) { /* out of memory */
            /* In contest environment a malloc failure is fatal – abort */
            abort();
        }
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n) {
    /* Static cache of primes found so far */
    static seq_t *primes = NULL;
    static size_t primes_len = 0;   /* number of primes stored */
    static size_t primes_cap = 0;   /* allocated capacity */
    static idx_t next_n = 2;        /* next integer n to test (n ≥ 2) */

    /* a(0) is undefined – return 0 to avoid crashes if called */
    if (n == 0) return 0;

    /* Expand cache until we have at least n primes */
    while (primes_len < (size_t)n) {
        /* Compute p = next_n⁴ – 3 using generic big_pow to avoid overflow */
        seq_t p = ipow((seq_t)next_n, (seq_t)4);
        p = p - (seq_t)3;

        if (is_prime_big(p)) {
            /* store prime in cache */
            ensure_capacity(primes_len + 1, &primes_cap, &primes);
            primes[primes_len++] = p;
        }
        ++next_n;
    }

    return primes[n - 1];
}
