/*
 * A107155  Primes of the form x^2 + 49y^2.
 * https://oeis.org/A107155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9785 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A107155: primes of the form x^2 + 49 y^2.
   The algorithm generates primes in ascending order using is_prime_uint64
   and tests each prime for the representation by brute‑force search over
   possible y.  A dynamic array stores the found primes so that any
   subsequent call to a(n) can be answered in O(1).  Complexity is
   roughly linear in n, with a modest constant factor. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

static seq_t *rep = NULL;          /* dynamic array of representable primes */
static size_t rep_cap = 0;          /* capacity of rep            */
static size_t rep_len = 0;          /* current number of entries  */

/* Ensure rep has at least new_cap slots, reallocating if necessary. */
static void ensure_rep_cap(size_t new_cap)
{
    if (new_cap <= rep_cap) return;
    size_t cap = rep_cap ? rep_cap : 64;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, rep, cap);
    rep = tmp;
    rep_cap = cap;
}

/* Append a prime to rep.  The caller guarantees that rep has space. */
static void add_rep(seq_t p)
{
    rep[rep_len++] = p;
}

/* Return the n-th prime p such that p = x^2 + 49*y^2, 1‑based index. */
seq_t a(idx_t n)
{
    static uint64_t cur = 2;         /* current candidate integer */
    static bool init = false;

    if (!init) {
        cur = 2;
        init = true;
    }

    /* Grow rep until we have at least n entries. */
    while (rep_len < (size_t)n) {
        /* Find next prime. */
        uint64_t p;
        do {
            if (cur <= 2) { cur = 3; }
            else          { cur += 2; }   /* skip even numbers except 2 */
            p = cur;
        } while (!is_prime_u64(p));

        /* Test representation p = x^2 + 49*y^2.  y can be zero only if p=0.   */
        uint64_t max_y = isqrt_u64(p / 49);
        bool ok = false;
        for (uint64_t y = 1; y <= max_y && !ok; ++y) {
            uint64_t t = p - 49 * y * y;
            uint64_t x = isqrt_u64(t);
            if (x * x == t) ok = true;
        }
        if (ok) {
            ensure_rep_cap(rep_len + 1);
            add_rep((seq_t)p);
        }
    }

    return rep[n - 1];
}
