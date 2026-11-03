/*
 * A067970  First differences of A014076, the odd nonprimes.
 * https://oeis.org/A067970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014076(n+1) - A014076(n) (first differences of odd
 *            nonprimes)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8646 (2026-11-03)
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

static seq_t *odd_nonprimes = NULL;   /* cache of odd nonprimes (1‑based, zero‑based internal) */
static idx_t  odd_nonprimes_count = 0;   /* number of elements currently cached */
static size_t odd_nonprimes_cap   = 0;   /* allocated capacity */

/* Ensure that the cache contains at least 'need' odd nonprimes. */
static void ensure_odd_nonprimes(idx_t need)
{
    if (odd_nonprimes_count >= need) return;

    /* Grow the cache if necessary. */
    if ((size_t)need > odd_nonprimes_cap) {
        size_t new_cap = odd_nonprimes_cap ? odd_nonprimes_cap * 2 : 8;
        while (new_cap < (size_t)need) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, odd_nonprimes, new_cap);
        if (!tmp) abort();
        odd_nonprimes = tmp;
        odd_nonprimes_cap = new_cap;
    }

    /* Start searching from the next odd integer after the last cached value. */
    seq_t candidate;
    if (odd_nonprimes_count == 0) {
        candidate = 1;                     /* 1 is the first odd nonprime */
    } else {
        candidate = odd_nonprimes[odd_nonprimes_count - 1] + (seq_t)2;
    }

    /* Collect odd nonprimes until we have the required amount. */
    while (odd_nonprimes_count < need) {
        if (!is_prime_big(candidate)) {
            odd_nonprimes[odd_nonprimes_count++] = candidate;
        }
        candidate += (seq_t)2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */
    ensure_odd_nonprimes(n + 1);             /* need (n)th and (n+1)th odd nonprimes */
    return odd_nonprimes[n] - odd_nonprimes[n - 1];
}
