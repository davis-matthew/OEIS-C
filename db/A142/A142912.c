/*
 * A142912  Primes congruent to 41 mod 63.
 * https://oeis.org/A142912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 931 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime congruent to 41 (mod 63).  a(1) = 41. */
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset is irrelevant */

    /* static cache of already found primes */
    static seq_t *cache = NULL;            /* dynamic array of terms */
    static idx_t  count = 0;               /* number of terms stored          */
    static idx_t  cap   = 0;               /* allocated capacity              */
    static seq_t  next  = (seq_t)41;       /* next candidate of the form 63*k+41 */

    if (count == 0)                       /* initialise lazily */
        next = (seq_t)41;

    /* grow cache if needed */
    if (count < n) {
        if (cap < n) {
            idx_t new_cap = cap ? (cap << 1) : 8;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
            if (!tmp) return 0;           /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* generate missing terms */
        while (count < n) {
            while (!is_prime_big(next))    /* keep searching until a prime */
                next = next + (seq_t)63;

            cache[count++] = next;         /* store the prime */
            next = next + (seq_t)63;       /* next candidate of the form 63*k+41 */
        }
    }

    return cache[n - 1];                  /* sequence is 1‑based */
}
