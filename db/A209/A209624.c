/*
 * A209624  Primes separated from their previous adjacent primes by a prime
 *          number of successive composites.
 * https://oeis.org/A209624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that the number of composites between p *
 *            and its preceding prime q equals a prime number, i.e. (p - q -
 *            1) is * prime. Returns the nth term of OEIS A209624
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 997 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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

/*  seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /*  Static cache of already found terms.  */
    static seq_t *terms   = NULL;
    static size_t  len    = 0;
    static size_t  cap    = 0;
    static uint64_t prev   = 2;      /* previous prime inspected   */
    static uint64_t current = 2;     /* last prime returned by next_prime */

    /*  Initialise on first call.  */
    if (len == 0) {
        cap   = 16;
        terms = mem_new(seq_t, cap);
    }

    /*  Keep generating primes until we have at least n terms stored.  */
    while (len < (size_t)n) {
        /*  Next prime after the previous one.  */
        current = next_prime(prev);

        /*  Number of composites between the two primes.  */
        uint64_t gap = current - prev;
        if (gap > 1 && is_prime_big((seq_t)(gap - 1))) {
            /*  Ensure capacity.  */
            if (len >= cap) {
                cap *= 2;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[len++] = (seq_t)current;   /* store as seq_t */
        }

        prev = current;   /* move to the prime we just processed   */
    }

    /*  Return the requested term (indices are 1‑based).  */
    return terms[(size_t)(n - 1)];
}
