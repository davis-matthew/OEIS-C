/*
 * A049561  Primes p such that x^29 = 2 has a solution mod p.
 * https://oeis.org/A049561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049561: primes p such that x^29 ≡ 2 (mod p) has a solution.
   A prime p ≠ 2 satisfies the condition iff either p ≠ 1 (mod 29)
   (then the 29‑th power map is bijective) or p ≡ 1 (mod 29) and
   2^((p‑1)/29) ≡ 1 (mod p); p = 2 is also a member of the sequence. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
/* next_prime returns the first prime strictly greater than its argument. */
#include "../utils/next_prime.h"
/* fast modular exponentiation for seq_t bases. */
#include "../utils/mod_pow.h"
/* dynamic array helpers. */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* cached sequence entries */
static seq_t *terms   = NULL;   /* array of primes found */
static size_t term_cnt = 0;     /* number of entries stored */
static size_t term_cap = 0;     /* current capacity of 'terms' */
/* last prime examined; start from 0 so that next_prime(0) yields 2. */
static uint64_t cur = 0;

/* ensure we have at least n terms stored in 'terms'. */
static void ensure_terms(idx_t n)
{
    /* lazily grow the array if needed. */
    while (term_cnt < (size_t)n)
    {
        /* get the next prime after the value currently stored in 'cur'. */
        cur = next_prime(cur);

        bool ok;
        if (cur == 2)
        {
            /* 0^29 ≡ 2 (mod 2). */
            ok = true;
        }
        else if ((cur - 1) % 29 == 0)
        {
            /* p ≡ 1 (mod 29): check whether 2 is a 29‑th power residue. */
            uint64_t exp = (cur - 1) / 29;
            seq_t val = mod_pow((seq_t)2, exp, (seq_t)cur);
            ok = (val == (seq_t)1);
        }
        else
        {
            /* For p with (p‑1) not divisible by 29 the map x → x^29 is bijective. */
            ok = true;
        }

        if (ok)
        {
            /* allocate more space if necessary. */
            if (terms == NULL || term_cnt == term_cap)
            {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                if (terms == NULL)
                    terms = seq_alloc(new_cap);
                else
                    terms = seq_realloc(terms, term_cap, new_cap);
                term_cap = new_cap;
            }
            terms[term_cnt++] = (seq_t)cur;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; on the off‑chance of n==0, return 0 safely */
    ensure_terms(n);
    return terms[n - 1];
}
