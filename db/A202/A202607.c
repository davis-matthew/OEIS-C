/*
 * A202607  Numbers n such that 6n and sigma(6n) are both a twin prime
 *          average.
 * https://oeis.org/A202607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is the nth positive integer n such that * 6n and σ(6n) are
 *            both twin‑prime averages * (i.e. 6n−1, 6n+1, σ(6n)−1 and σ(6n)+1
 *            are all prime)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /*  Cache the sequence values computed so far.  */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;     /* number of terms stored   */
    static size_t cache_cap  = 0;     /* current capacity of cache*/
    static idx_t  tested_n   = 1;     /* next n to test           */

    /*  The requested term may not yet be in the cache.  */
    while (cache_len < n) {

        /*  Search for the next valid n.  */
        while (1) {
            idx_t m = 6ULL * tested_n;   /* m = 6·n */

            /*  Quick small‑prime test on the twin‑prime pair around 6n.  */
            if (is_prime_seq(m - 1) && is_prime_seq(m + 1)) {

                /*  Compute σ(6n).  */
                seq_t sigma = sigma_of(m);

                /*  Quick big‑prime test on the twin‑prime pair around σ(6n).  */
                if (is_prime_big(sigma - 1) && is_prime_big(sigma + 1)) {
                    /*  Found a term: store it.  */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? 2 * cache_cap : 8;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_len++] = (seq_t)tested_n;
                }
            }

            /*  Move to the next candidate regardless of outcome.  */
            tested_n++;

            /*  If we have enough terms, exit the inner loop.  */
            if (cache_len >= n) break;
        }
    }

    /*  Return the requested term.  */
    return cache[n - 1];
}
