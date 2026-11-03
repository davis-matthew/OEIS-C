/*
 * A216075  Numbers n such that 2*prime(n) - prime(n+1) is prime.
 * https://oeis.org/A216075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 652, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 993 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216075:  Numbers n such that 2*prime(n) - prime(n+1) is prime
 *  a(n) is the n‑th such integer (a(1)=3, a(2)=4, …).
 *  The implementation builds a table of primes once and caches the
 *  sequence values.  It works for arbitrary‑precision seq_t, uint64_t,
 *  and other supported integer types.  The code respects the
 *  requirements for C11 and C++17 without using <new> or other
 *  non‑portable constructs.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

/* --- Static state ---------------------------------------------------- */
static uint64_t *prime_values = NULL;   /* prime list, 1‑based: prime_values[0]=2   */
static size_t   prime_count    = 0;      /* number of primes stored                */
static idx_t    prime_limit    = 1000000;/* current upper bound passed to build_primes_upto */

static seq_t *seq_cache   = NULL;        /* cached sequence values */
static size_t seq_len     = 0;           /* number of terms stored   */
static size_t seq_cap     = 0;           /* allocated capacity       */

/* last candidate n that has been checked (0 means none checked yet) */
static idx_t checked_n = 0;

/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    /* build primes up to the initial limit */
    build_primes_upto(prime_limit, &prime_values, &prime_count);
}

/* Ensure we have at least (needed + 1) primes (indices 1 … needed+1) */
static void ensure_primes(idx_t needed)
{
    if (prime_count > (size_t)needed)
        return;
    /* Double the limit until it is sufficient. */
    while (prime_count <= (size_t)needed)
    {
        prime_limit *= 2;
        build_primes_upto(prime_limit, &prime_values, &prime_count);
    }
}

/* Grow the sequence cache to hold at least n terms (1‑based). */
static void grow_cache(size_t n)
{
    if (n <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < n)
        new_cap <<= 1;
    /* realloc is allowed in C++ after a C‑style cast */
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap   = new_cap;
}

/* Generate terms up to n.  After this, seq_cache contains at least n terms. */
static void compute_up_to(idx_t n)
{
    if (!prime_values)
        init_primes();

    grow_cache((size_t)n);

    while (seq_len < (size_t)n)
    {
        ++checked_n;                    /* candidate n (1‑based) */
        /* Ensure primes up to checked_n+1 exist */
        ensure_primes(checked_n);

        /* access primes[checked_n-1] == prime(checked_n)
           and primes[checked_n]   == prime(checked_n+1)            */
        seq_t p = (seq_t)prime_values[checked_n - 1];
        seq_t q = (seq_t)prime_values[checked_n];

        /* 2*prime(checked_n) - prime(checked_n+1) */
        seq_t val = (seq_t)(( (seq_t)2 * p ) - q);

        if (is_prime_big(val))
            seq_cache[seq_len++] = (seq_t)checked_n; /* store the integer n */
    }
}

/* --------------------------------------------------------------------- */
/* Public interface required by the build system                      */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined, but harmless */
    compute_up_to(n);
    return seq_cache[n - 1];
}
