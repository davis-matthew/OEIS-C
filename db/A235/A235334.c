/*
 * A235334  Numbers n such that for any positive integers (a, b), if a * b = n
 *          then a + b is a square.
 * https://oeis.org/A235334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 621, u128 n <= 1163,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 1063 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 3 for n = 1;
   for n > 1, a(n) is the product (2k²‑1)(2k²+1) where k ≥ 2 and both
   2k²‑1 and 2k²+1 are prime.  This is the characterization of the
   OEIS sequence A235334.
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

/* cache of previously computed terms ------------------------------------ */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of stored terms   */
static uint64_t last_k = 0;          /* last k that was examined */

/* append a term to the cache, growing it if necessary ------------------- */
static void cache_append(seq_t val)
{
    if (cache_len == 0) {
        cache = mem_new(seq_t, 1);
        if (!cache) abort();
    } else {
        /* grow: double the capacity each time */
        cache = mem_resize(seq_t, cache, (cache_len + 1));
        if (!cache) abort();
    }
    cache[cache_len++] = val;
}

/* fill the cache until it contains at least n terms ------------------- */
static void ensure_up_to(idx_t n)
{
    while (cache_len < n) {
        /* first term is 3 */
        if (cache_len == 0) {
            cache_append((seq_t)3);
            last_k = 1;               /* next k to test is 2 */
            continue;
        }

        /* test next k */
        ++last_k;
        seq_t k = (seq_t)last_k;

        /* compute p = 2k²‑1, q = 2k²+1 */
        seq_t kk = k * k;          /* k²   */
        seq_t p  = kk * 2 - 1;     /* 2k²‑1 */
        seq_t q  = p + 2;          /* 2k²+1 */

        if (is_prime_big(p) && is_prime_big(q)) {
            /* term is n = p * q  = (2k²‑1)(2k²+1) = 4k⁴‑1 */
            seq_t n_val = p * q;
            cache_append(n_val);
        }
    }
}

/* public routine -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */
    ensure_up_to(n);
    return cache[n - 1];
}
