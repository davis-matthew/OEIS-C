/*
 * A139927  Primes of the form 19x^2+14xy+19y^2.
 * https://oeis.org/A139927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 19x²+14xy+19y². Equivalently,
 *            the primes are exactly those congruent to 19, 67, 115, 163, 187,
 *            or 307 modulo 312
 * Range:     u8 n <= 3, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9476 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static inline bool is_target_residue(seq_t v) {
    seq_t r = v % 312;
    return r == 19 || r == 67 || r == 115 ||
           r == 163 || r == 187 || r == 307;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for offset 1 */

    /* persistent static data: list of primes of the target form */
    static seq_t *cands      = NULL;
    static size_t  cands_cap = 0;
    static size_t  cands_used = 0;
    static seq_t  next_start = 2;      /* first integer to test */

    /* initialise storage on first call */
    if (!cands) {
        cands_cap = 16;
        cands     = mem_new(seq_t, cands_cap);
        if (!cands) return 0; /* allocation failure */
    }

    /* generate primes until we have at least n terms */
    while (cands_used < n) {
        for (;;) {
            seq_t candidate = next_start++;
            if (!is_target_residue(candidate))
                continue;
            if (!is_prime_big(candidate))
                continue;

            /* grow array if needed */
            if (cands_used == cands_cap) {
                cands_cap <<= 1;               /* double capacity */
                seq_t *tmp = mem_resize(seq_t, cands, cands_cap);
                if (!tmp) return 0;            /* allocation failure */
                cands = tmp;
            }
            cands[cands_used++] = candidate;
            break;
        }
    }

    return cands[n - 1];
}
