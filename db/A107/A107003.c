/*
 * A107003  Primes of the form 24*k + 5.
 * https://oeis.org/A107003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A107003:  Primes of the form 24*k + 5.
 *  a(n) = the n-th prime congruent to 5 (mod 24).  The sequence is
 *  generated incrementally and cached so that calls can arrive in any
 *  order.  Miller–Rabin with the deterministic 64–bit witness set
 *  is used for primality testing. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_mod.h"

/* --- prime storage ---------------------------------------------- */

static seq_t *prime_form = NULL;   /* dynamic array of primes 24*k+5   */
static size_t prime_form_cnt = 0;  /* number of primes stored        */
static size_t prime_form_cap = 0;  /* current capacity of the array  */
static uint64_t next_candidate = 5; /* next integer of form 24*k+5    */

/* --- modular arithmetic helpers --------------------------------- */

static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* --- deterministic 64‑bit Miller–Rabin ---------------------------- */

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if ((n & 1) == 0) return n == 2;
    static const uint64_t small[] =
        {3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return 1;
        if (n % small[i] == 0) return 0;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        int cont = 0;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n-1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* --- main sequence function ------------------------------------- */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* off‑by‑zero undefined   */

    /* grow the cache until we have at least n primes            */
    while (prime_form_cnt < n)
    {
        if (is_prime_uint64(next_candidate))
        {
            if (prime_form_cnt == prime_form_cap)
            {
                size_t new_cap = prime_form_cap ? prime_form_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_form, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                prime_form = tmp;
                prime_form_cap = new_cap;
            }
            prime_form[prime_form_cnt++] = (seq_t)next_candidate;
        }
        next_candidate += 24;
    }
    return prime_form[n - 1];
}
