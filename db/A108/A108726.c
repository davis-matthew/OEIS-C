/*
 * A108726  Numbers n such that 11*n + 29 is prime.
 * https://oeis.org/A108726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A108726:  n such that 11*n + 29 is prime.
 *  The function returns the n-th smallest non‑negative integer n
 *  with 11*n + 29 prime (a(1)=0).  It memoises the terms as they are
 *  discovered, so calls may come in any order.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* --- Miller–Rabin for 64‑bit integers ---------------------------------- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((__uint128_t)a * b % m);
}
static inline uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t m)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mul_mod64(r, a, m);
        a = mul_mod64(a, a, m);
        d >>= 1;
    }
    return r;
}
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small/sizeof *small; ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases/sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------- */
/*  Function a(idx_t n)                                                   */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1, but guard for safety */

    /* Static state for memoisation ------------------------------------- */
    static seq_t *terms = NULL;      /* array holding the terms found so far */
    static idx_t terms_cap = 0;      /* current capacity of the array      */
    static idx_t terms_cnt = 0;      /* number of terms stored            */
    static uint64_t next_cand = 0;   /* next candidate n to test         */

    /* Grow the array until we have at least n terms ------------------- */
    while (terms_cnt < n)
    {
        uint64_t cand = next_cand;
        uint64_t val = 11ULL * cand + 29ULL;
        if (is_prime_uint64(val))
        {
            /* Ensure there is space --------------------------------- */
            if (terms_cnt == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failed */
                terms   = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)cand;
        }
        ++next_cand;
    }

    return terms[n-1];
}
