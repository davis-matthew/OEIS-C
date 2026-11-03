/*
 * A145050  Primes p of the form 4*k+1 for which s=26 is the least positive
 *          integer such that s*p-(floor(sqrt(s*p)))^2 is a square.
 * https://oeis.org/A145050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p ≡ 1 (mod 4) for which s = 26 is the *
 *            smallest positive integer such that * s·p – floor(√(s·p))² * is
 *            a perfect square, and no smaller positive s yields a square. *
 *            The function searches primes sequentially, testing the condition
 *            for * each candidate, and memoises the fou...
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    static seq_t *found = NULL;   /* array of found primes */
    static size_t len   = 0;      /* number of primes stored   */
    static size_t cap   = 0;      /* current allocation cap   */
    static seq_t cand   = 5;      /* next odd candidate       */
    static bool init    = false;  /* one‑time allocation flag */

    /* One‑time allocation of the dynamic array */
    if (!init) {
        cap   = 16;
        found = mem_new(seq_t, cap);
        if (!found) abort();
        init = true;
    }

    /* Generate more primes until we have at least n of them */
    while (len < n) {
        /* Find the next prime that is ≡ 1 (mod 4) */
        while (!((cand & 3u) == 1u && isPrimeGeneric(cand)))
            cand += 2u;               /* stay odd */

        seq_t p = cand;                /* candidate prime */

        /* Check that 1 ≤ s ≤ 25 do NOT produce a square */
        bool ok = true;
        for (idx_t s = 1; s <= 25; ++s) {
            seq_t prod  = p * (seq_t)s;
            seq_t r1    = isqrt_seq(prod);
            seq_t d     = prod - r1 * r1;
            seq_t r2    = isqrt_seq(d);
            if (r2 * r2 == d) {      /* d is a perfect square */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Verify that s = 26 indeed yields a square */
            seq_t prod  = p * (seq_t)26;
            seq_t r1    = isqrt_seq(prod);
            seq_t d     = prod - r1 * r1;
            seq_t r2    = isqrt_seq(d);
            if (r2 * r2 == d) {
                /* Store the prime in the array */
                if (len == cap) {
                    size_t new_cap = cap << 1;
                    seq_t *tmp = mem_resize(seq_t, found, new_cap);
                    if (!tmp) abort();
                    found = tmp;
                    cap   = new_cap;
                }
                found[len++] = p;
            }
        }

        cand += 2u;                    /* next odd number to test */
    }

    return found[n - 1];
}
