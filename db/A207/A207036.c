/*
 * A207036  Numbers k such that the sum of divisors of k is a multiple of 24.
 * https://oeis.org/A207036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8134 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207036: Numbers k such that the sum of divisors of k is a
   multiple of 24.  The sequence is computed by brute‑force search
   testing each integer k in order and recording those for which
   σ(k) ≡ 0 (mod 24).  The search is memoised so that calls to a(n)
   are answered in O(1) after the range has been generated.  */

/* include basic headers – only <stdlib.h> is strictly needed,
   but <stdint.h> may be required for UINT64_MAX in some
   assembler contexts. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Forward declaration of helper: compute σ(k) mod 24 using simple trial
   factorisation.  The result is in the range [0,23]. */
static int sigma_mod24(uint64_t n)
{
    uint64_t m = n;
    int sigma_mod = 1;           /* modulo 24 product of component sums */

    /* try factor 2 separately for efficiency */
    if ((m & 1) == 0) {
        int e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        /* sum = 1 + 2 + 2^2 + ... + 2^e (mod 24) */
        int term = 1;            /* sum of powers of 2 */
        int pow = 1;             /* current power 2^i */
        for (int i = 0; i < e; ++i) {
            pow = (pow * 2) % 24;
            term = (term + pow) % 24;
        }
        sigma_mod = (sigma_mod * term) % 24;
    }

    /* test odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* sum = 1 + p + p^2 + ... + p^e (mod 24) */
        int term = 1;
        int pow = 1;
        int pmod = (int)(p % 24);
        for (int i = 0; i < e; ++i) {
            pow = (pow * pmod) % 24;
            term = (term + pow) % 24;
        }
        sigma_mod = (sigma_mod * term) % 24;
    }

    /* if a prime factor remains */
    if (m > 1) {
        int term = (1 + (int)(m % 24)) % 24;
        sigma_mod = (sigma_mod * term) % 24;
    }

    return sigma_mod;
}

/* Memoised storage of the sequence terms. */
static seq_t *s_cache = NULL;
static size_t s_len   = 0;   /* number of stored terms */
static size_t s_cap   = 0;   /* allocated capacity */
static uint64_t next_k = 1;  /* next integer to test */

/* ensure sufficient capacity for at least need elements */
static void ensure_capacity(size_t need)
{
    if (s_cap >= need)
        return;
    size_t newcap = s_cap ? s_cap * 2 : 8;
    if (newcap < need)
        newcap = need;
    seq_t *newptr = mem_resize(seq_t, s_cache, newcap);
    if (!newptr) abort();   /* out of memory – abort is acceptable here */
    s_cache = newptr;
    s_cap   = newcap;
}

/* public function: nth term of A207036 */
seq_t a(idx_t n)
{
    if (n == 0)  /* Not part of the OEIS offset – return 0 for safety. */
        return 0;

    /* generate terms until we have at least n of them */
    while (s_len < n) {
        /* test current value of next_k for σ(k) ≡ 0 mod 24 */
        if (sigma_mod24((uint64_t)next_k) == 0) {
            ensure_capacity(s_len + 1);
            s_cache[s_len++] = (seq_t)next_k;   /* store the term */
        }
        ++next_k;   /* move to next integer */
    }

    return s_cache[n - 1];   /* zero‑based indexing in cache */
}
