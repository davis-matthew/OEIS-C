/*
 * A235156  Primes which have one or more occurrences of exactly four
 *          different digits.
 * https://oeis.org/A235156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 927 (2026-11-03)
 * Helpers:   digit_counts, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with exactly four distinct decimal digits. */

/* We use the provided utilities for digit counting and primality testing. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
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
#include <stdlib.h>

/* Dynamically enlarge an array of seq_t if needed. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    static seq_t *cached    = NULL;            /* cached sequence */
    static size_t cachedlen = 0, cachedcap = 0;
    static seq_t current = 2;                  /* next candidate to test */

    while (cachedlen < (size_t)n) {
        /* Test primality */
        if (is_prime_big(current)) {
            unsigned char cnt[10] = {0};
            digit_counts(current, cnt);
            int distinct = 0;
            for (int d = 0; d < 10; ++d) if (cnt[d]) ++distinct;
            if (distinct == 4) {
                ensure_capacity(&cached, &cachedcap, cachedlen + 1);
                cached[cachedlen++] = current;
            }
        }
        ++current;
    }
    return cached[(size_t)n - 1];
}
