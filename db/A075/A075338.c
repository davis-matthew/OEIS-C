/*
 * A075338  a(1) = 1, a(2n) = smallest prime obtained by adding a composite
 *          number to a(2n-1). a(2n+1) = the smallest composite number
 *          obtained by adding a prime to a(2n).
 * https://oeis.org/A075338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075338
   a(1) = 1
   For n >= 2 :
     if n is even: a(n) = smallest prime that equals a(n-1) + c,
                   where c is a composite number;
     if n is odd:  a(n) = smallest composite that equals a(n-1) + p,
                   where p is a prime number.
*/

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

/* static cache of computed terms */
static seq_t *cache = NULL;      /* cache[0] holds a(1) */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_len = 0;     /* number of terms currently cached */

/* Ensure cache has room for at least n terms (n >= 1) */
static void ensure_capacity(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;
    /* realloc returns void* in C++ – cast explicitly */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();              /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n >= 1.  The function is memoized. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined below offset, but safe */

    size_t idx = (size_t)n;          /* 1‑based index of the term to compute */

    ensure_capacity(idx);

    /* Seed the cache if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms sequentially */
    for (size_t i = cache_len; i < idx; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sum;
        /* i is 0‑based; term index is i+1 */
        if (((i + 1) & 1) == 0) {            /* even index: need a prime sum */
            uint64_t c = 4;                  /* first composite */
            for (;;) {
                if (!is_prime_seq((seq_t)c)) {   /* c is composite */
                    sum = prev + (seq_t)c;
                    if (is_prime_big(sum))
                        break;
                }
                ++c;
            }
            cache[i] = sum;
        } else {                               /* odd index: need a composite sum */
            uint64_t p = 2;                  /* first prime */
            for (;;) {
                if (is_prime_seq((seq_t)p)) {     /* p is prime */
                    sum = prev + (seq_t)p;
                    if (!is_prime_big(sum))
                        break;
                }
                ++p;
            }
            cache[i] = sum;
        }
    }

    cache_len = idx;
    return cache[n - 1];
}
