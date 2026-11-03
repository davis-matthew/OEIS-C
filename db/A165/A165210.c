/*
 * A165210  Primes of the form (6^m - 1)/5.
 * https://oeis.org/A165210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6^m - 1)/5, where m is the n-th exponent from A004062
 *            (prime exponents for which the repunit in base 6 is prime)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function maintains a static cache of computed primes and
   iteratively generates the next repunit by the recurrence
   rep(m+1) = 6*rep(m) + 1, starting from rep(1) = 1.
   It tests each prime exponent with is_prime_small and each
   resulting repunit with is_prime_big.  For fixed‑size seq_t,
   multiplication is performed with an unsigned __int128
   intermediate to detect overflow and stop the search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static state ----------------------------------------------------- */
    static seq_t *cache = NULL;          /* array of found primes */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static seq_t repunit = 0;            /* repunit for current exponent m */
    static idx_t    current_m = 0;       /* current exponent (starts at 0) */

    /* Initialise on first call ----------------------------------------- */
    if (repunit == 0) {
        repunit = 1;                     /* (6^1 - 1)/5 = 1 (not prime) */
        current_m = 1;
    }

    /* Return cached value if already computed --------------------------- */
    if (n <= cache_len) return cache[n - 1];

    /* Grow cache if necessary ------------------------------------------ */
    if (cache_cap == 0 || cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;           /* allocation failure handled gracefully */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have the n‑th one ------------------------ */
    while (cache_len < n) {
        /* advance to next exponent */
        current_m++;

        /* compute repunit = previous_repunit * 6 + 1, guard against overflow */
        seq_t new_repunit;
        bool overflow = false;
        if (sizeof(seq_t) <= 8) {
            /* fixed-size integer: use 128‑bit intermediate for safety */
            unsigned __int128 temp = (unsigned __int128)repunit * 6u + 1u;
            unsigned __int128 max   = (unsigned __int128)~((seq_t)0);
            if (temp > max) overflow = true;
            new_repunit = (seq_t)temp;
        } else {
            /* arbitrary‑precision type: multiplication cannot overflow */
            new_repunit = repunit * 6 + 1;
        }
        if (overflow) {
            /* Subsequent terms would exceed seq_t range */
            break;
        }
        repunit = new_repunit;

        /* Only prime exponents can produce a prime repunit */
        if (!is_prime_seq((uint64_t)current_m))
            continue;

        /* Test primality of the repunit */
        if (is_prime_big(repunit)) {
            cache[cache_len++] = repunit;
        }
    }

    /* If we failed to find the required term, return 0 */
    if (n > cache_len) return 0;
    return cache[n - 1];
}
