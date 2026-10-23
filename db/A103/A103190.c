/*
 * A103190  Numbers k such that k29 is prime.
 * https://oeis.org/A103190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103190 – Numbers k such that the decimal concatenation k29 is prime.
   The function builds the sequence lazily, caching each found k and
   re‑using the cache for out‑of‑order calls.  A deterministic
   Miller–Rabin test for 64‑bit integers is used.  The implementation
   relies only on standard headers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */
static uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;
    if (n % 5 == 0)       return n == 5;
    if (n % 7 == 0)       return n == 7;
    if (n % 11 == 0)      return n == 11;
    if (n % 13 == 0)      return n == 13;

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* bases sufficient for all 64‑bit integers */
    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (unsigned j = 1; j < s; ++j) {
            x = mul_mod_u64(x, x, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- cached generation of the sequence ---------- */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* array of found k values */
    static idx_t  used = 0;       /* number of cached elements */
    static idx_t  capacity = 0;   /* allocated size of cache   */
    static idx_t  next_k = 0;     /* next candidate k to test */

    /* grow the cache if needed */
    if (capacity < n && used < n) {
        idx_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 as fallback */
            return (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* generate terms until we have at least n terms */
    while (used < n) {
        /* compute k29 = k*100 + 29 without overflow */
        uint64_t candidate = (uint64_t)next_k * 100ULL + 29ULL;
        if (is_prime_uint64(candidate)) {
            cache[used] = (seq_t)next_k;
            used++;
        }
        next_k++;
    }

    /* return the (n‑1)th element (a(1) is cache[0]) */
    return cache[n - 1];
}
