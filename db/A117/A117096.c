/*
 * A117096  Numbers n such that nextprime(2*n)<2*nextprime(n) (if p is prime
 *          then nextprime(p)=p).
 * https://oeis.org/A117096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117096
 *  a(n) = the n‑th positive integer m such that
 *         nextprime(2*m) < 2*nextprime(m)
 *  (if p is prime then nextprime(p)=p).
 *
 *  Implementation:  a linear search with a helper that returns the
 *  smallest prime ≥ a given start.  The helper uses the supplied
 *  is_prime_big() routine from the utils package.  The sequence is
 *  memoised so that calls in any order remain fast.
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

static uint64_t _next_prime_uint64(uint64_t start)
{
    for (uint64_t p = start; ; ++p)
        if (is_prime_big((seq_t)p))
            return p;
}

seq_t a(idx_t n)
{
    /* memoised list of already found terms */
    static seq_t *terms = NULL;
    static size_t    len  = 0;   /* number of stored terms  */
    static size_t    cap  = 0;   /* current capacity         */
    static uint64_t  next = 1;   /* next integer to test    */

    /* grow the stored list as required */
    while ((size_t)n > len)
    {
        uint64_t c = next++;   /* candidate integer c      */

        uint64_t p = _next_prime_uint64(c);          /* nextprime(c)    */
        uint64_t q = _next_prime_uint64(2 * c);      /* nextprime(2c)   */

        if (q < 2 * p)                            /* property holds */
        {
            if (len == cap)                      /* grow storage   */
            {
                size_t newcap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, terms, newcap);
                if (!tmp) abort();                /* allocation fail */
                terms = tmp;
                cap   = newcap;
            }
            terms[len++] = (seq_t)c;              /* store term     */
        }
    }

    return terms[n - 1];   /* 1‑based indexing into the sequence */
}
