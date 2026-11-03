/*
 * A125283  Primes p for which there exists no integer n with 1 < n^2 < p such
 *          that n^4 - n^2 + p is prime (i.e., primes that don't arise as q's
 *          in A126769).
 * https://oeis.org/A125283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth prime p such that for every integer n>1 with *
 *            n^2 < p, the number n^4 − n^2 + p is composite (i.e., no such n
 *            exists)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"
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

/* Computes the n‑th element (1‑based) of OEIS sequence A125283. */
seq_t a(idx_t n)
{
    static seq_t *seq     = NULL;   /* stored sequence values               */
    static size_t seq_len = 0;      /* number of stored values             */
    static size_t seq_cap = 0;      /* current array capacity              */
    static idx_t  last_prime = 1;   /* last prime examined                 */

    /* Initialise storage on first call. */
    if (!seq) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) return 0; /* allocation failure – return 0 as a safe fallback */
    }

    /* Generate primes until we have at least n terms in the sequence. */
    while (seq_len < n) {
        /* Find the next prime > last_prime. */
        idx_t cand = last_prime + 1;
        while (!is_prime(cand)) {
            ++cand;
        }

        /* Test whether cand satisfies the A125283 property. */
        bool ok = true;
        idx_t maxn = isqrt(cand - 1);  /* floor(sqrt(p-1)) */

        for (idx_t i = 2; i <= maxn && ok; ++i) {
            unsigned __int128 n128 = (unsigned __int128)i;
            unsigned __int128 n2   = n128 * n128;        /* n^2                */
            unsigned __int128 val128 = n2 * n2            /* n^4                */
                                      + (unsigned __int128)cand
                                      - n2;                 /* n^4 - n^2 + p     */
            seq_t val = (seq_t)val128;   /* safe cast: val <= 2^64-1 for p <= 4.3e9 */
            if (is_prime_big(val)) {
                ok = false;  /* found a suitable n → discard cand */
            }
        }

        if (ok) {
            /* Ensure capacity. */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
                if (!seq) return 0; /* allocation failure – return 0 */
            }
            seq[seq_len++] = (seq_t)cand;
        }

        last_prime = cand;  /* advance to the prime just processed. */
    }

    return seq[n - 1];
}
