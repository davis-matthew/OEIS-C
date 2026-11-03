/*
 * A213049  Primes p such that the order of 2 mod p is a square.
 * https://oeis.org/A213049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 178, u64 n <= 178,
 *            u128 n <= 178, big any n
 * Validated: standard, pass, 58 terms through n = 164 (2026-11-03)
 * Helpers:   factor_u64, is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p (n >= 1) such that
 * the multiplicative order of 2 modulo p is a perfect square.
 *
 * The algorithm:
 *  1. Generate odd prime candidates in ascending order.
 *  2. For each candidate p, factor p-1.
 *  3. Using the prime factorisation of p-1, compute ord_2(p)
 *     by repeatedly dividing by each prime factor while the
 *     corresponding power still satisfies 2^k ≡ 1 (mod p).
 *  4. If ord_2(p) is a perfect square then store p in a memoised
 *     static array.
 *  5. When the array contains at least n elements, return the (n-1)-th
 *     element.
 *
 * Note:  seq_t may be a C integer type or an arbitrary‑precision
 *        integer class.  Conversions from uint64_t to seq_t are carried
 *        out via seq_from_str() so that the code works for both kinds.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static seq_t seq_from_uint64(uint64_t v)
{
    /* Convert a uint64_t value to seq_t using a temporary string. */
    char buf[21]; /* 20 digits + '\0' */
    int len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
    return seq_from_str(buf);
}

/* Integer square‑root by binary search; works for all 0 <= n <= 2^64‑1. */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
        {
            if ((mid + 1) * (mid + 1) > n) return mid;
            lo = mid + 1;
        }
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* --- static memoised data ------------------------------------------ */
    static seq_t *good = NULL;          /* array of found primes          */
    static size_t good_len = 0;         /* number stored                  */
    static size_t good_cap = 0;         /* current capacity               */
    static uint64_t next_candidate = 5; /* last prime candidate examined  */

    /* --- factorisation workspace ---------------------------------------- */
    static uint64_t pf_pr[64];
    static uint64_t pf_ex[64];
    size_t pf_cnt = 0;

    /* --- generate new terms until we have at least n --------------------- */
    while (good_len < (size_t)n)
    {
        /* find next odd prime candidate --------------------------------- */
        while (!is_prime(next_candidate))
            next_candidate += 2;

        uint64_t p = next_candidate;  /* candidate prime */

        /* factor p-1 ----------------------------------------------------- */
        uint64_t t = p - 1;
        factor_u64(t, pf_pr, pf_ex, &pf_cnt);

        /* compute multiplicative order of 2 modulo p --------------------- */
        uint64_t ord = t;
        for (size_t i = 0; i < pf_cnt; ++i)
        {
            uint64_t q = pf_pr[i];
            while (ord % q == 0)
            {
                uint64_t cand = ord / q;
                if (powmod((seq_t)2, cand, (seq_t)p) == (seq_t)1)
                    ord = cand;
                else
                    break;
            }
        }

        /* check whether ord is a perfect square -------------------------- */
        uint64_t root = isqrt_uint64(ord);
        if (root * root == ord)
            append_candidate(&good, &good_len, &good_cap, seq_from_uint64(p));

        /* move to next candidate ---------------------------------------- */
        next_candidate += 2;
    }

    return good[(size_t)n - 1];
}
