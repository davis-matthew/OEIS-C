/*
 * A038905  31 is a square mod p.
 * https://oeis.org/A038905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 989 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A038905
 *
 *  a(n) = the nth prime p (1‑based) for which 31 is a quadratic residue modulo p.
 *  The sequence includes the prime 31 itself.
 *
 *  Implementation: incremental trial‑division prime generation with a dynamic
 *  list of primes and a dynamic list of qualifying primes.  No external
 *  helper functions except the provided jacobi_symbol() routine.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Dynamic array of primes */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Dynamic array of qualifying primes (the sequence itself) */
static seq_t *qual = NULL;
static size_t qual_len = 0;
static size_t qual_cap = 0;

/* Next number to test for primality.  Starts at 2. */
static uint64_t next_candidate = 2;

/* Grow a dynamic array; doubles capacity or sets to 4 if empty. */
#ifdef __cplusplus
template <class T>
static void grow_array(T **arr, size_t *len, size_t *cap, size_t elem_size)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        T *new_ptr = mem_resize(T, *arr, (new_cap * elem_size) / sizeof(T));
        if (!new_ptr) { /* on allocation failure return the old pointer */
            return;
        }
        *arr = new_ptr;
        *cap = new_cap;
    }
}
#else
static void grow_array(void **arr, size_t *len, size_t *cap, size_t elem_size)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        void *new_ptr = realloc(*arr, new_cap * elem_size);
        if (!new_ptr) { /* on allocation failure return the old pointer */
            return;
        }
        *arr = new_ptr;
        *cap = new_cap;
    }
}
#endif

/* Append a prime to the prime list */
static void add_prime(uint64_t p)
{
    grow_array(OEISC_VPP(&primes), &primes_len, &primes_cap, sizeof(uint64_t));
    primes[primes_len++] = p;
}

/* Append a qualifying prime to the sequence list */
static void add_qual(seq_t p)
{
    grow_array(OEISC_VPP(&qual), &qual_len, &qual_cap, sizeof(seq_t));
    qual[qual_len++] = p;
}

/* Generate primes and qualifying primes until we have at least n results. */
static void generate(idx_t n)
{
    if (qual_len >= n) return;  /* already enough */

    while (qual_len < n) {
        uint64_t p = next_candidate;

        /* Check primality by trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t q = primes[i];
            if (q * q > p) break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            add_prime(p);
            if (p == 31 || jacobi_symbol(31, p) == 1) {
                add_qual((seq_t)p);
            }
        }

        /* Increment candidate: after handling 2, skip even numbers. */
        if (p == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* Return the n‑th term (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe for callers */
    generate(n);
    return qual[n - 1];
}
