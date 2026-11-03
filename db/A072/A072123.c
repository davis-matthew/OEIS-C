/*
 * A072123  Remainder when Fibonacci(n) is divided by prime(n).
 * https://oeis.org/A072123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6583, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9270 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = F(n) mod prime(n)
 * (F(n) is the nth Fibonacci number, prime(n) is the nth prime, indexing starts at 1)
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t fib_mod_u32(idx_t n, uint32_t mod)
{
    /* Fast doubling algorithm for Fibonacci mod 'mod'.
       Returns F(n) mod mod.  n is 0‑based (F(0)=0, F(1)=1). */
    uint64_t m = mod;
    uint64_t a = 0;   /* F(0) */
    uint64_t b = 1;   /* F(1) */
    /* iterate over bits of n from most significant to least */
    for (int i = 63 - __builtin_clzll(n); i >= 0; --i)
    {
        /* c = F(2k) = a * (2*b - a)  mod m */
        uint64_t temp = (2 * b) % m;
        uint64_t t = (temp + m - a) % m;   /* 2*b - a mod m, ensure non‑negative */
        uint64_t c = ((__int128)a * t) % m;

        /* d = F(2k+1) = a*a + b*b  mod m */
        uint64_t d = (( (__int128)a * a ) % m + ( (__int128)b * b ) % m) % m;

        if ((n >> i) & 1)
        {
            a = d;                    /* F(2k+1) */
            b = (c + d) % m;          /* F(2k+2) = F(2k)+F(2k+1) */
        }
        else
        {
            a = c;                    /* F(2k)   */
            b = d;                    /* F(2k+1) */
        }
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    /* offset is 1, so n >= 1 for valid calls */
    uint32_t p = get_prime(n);
    return fib_mod_u32(n, p);
}
