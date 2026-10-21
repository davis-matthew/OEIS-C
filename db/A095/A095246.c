/*
 * A095246  a(n) is chosen to be the least number such that concatenation
 *          a(1)a(2)a(3)...a(n-1)a(n) is congruent to n (mod prime(n)).
 * https://oeis.org/A095246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the least integer x such that * concatenation
 *            a(1)a(2)…a(n) ≡ n (mod prime(n))
 * Range:     u8 n <= 48, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_len, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the terms incrementally,
 * caching already produced values and computing each new term
 * by a direct search on increasing digit length. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_prime.h"   /* seq_t get_prime(idx_t n);      */
#include "../utils/digit_len.h"   /* int  digit_len(seq_t v);       */

/* ------------------------------------------------------------------ */
/*  helper to compute (10^len) mod mod_value                           */
static seq_t pow10mod(size_t len, seq_t mod_value)
{
    seq_t result = 1 % mod_value;
    for (size_t i = 0; i < len; ++i)
        result = (result * 10) % mod_value;
    return result;
}

/* ------------------------------------------------------------------ */
/*  dynamic cache for already computed a(i) values                     */
static seq_t *a_cache    = NULL; /* storage of terms a(1), a(2), …               */
static size_t a_cap      = 0;    /* allocated capacity                        */
static size_t a_computed = 0;    /* number of terms actually computed         */

/* enlarge cache capacity to hold at least 'need' terms                 */
static void ensure_a_capacity(size_t need)
{
    if (a_cap >= need) return;
    size_t cap = a_cap ? a_cap : 16;
    while (cap < need) cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    a_cap   = cap;
}

/* ------------------------------------------------------------------ */
/*  main sequence generator                                            */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* make sure we can store up to n terms */
    ensure_a_capacity((size_t)n);

    /* return cached value if already known */
    if (a_computed >= n)
        return a_cache[n - 1];

    /* compute missing terms one by one */
    for (idx_t k = a_computed; k < n; ++k)
    {
        /* prime number for term k+1 (k is 0‑based) */
        seq_t prime = get_prime(k + 1);

        /* build the concatenation of a(1)…a(k) modulo 'prime'    */
        seq_t cur_mod = 0;
        for (idx_t i = 0; i < k; ++i)
        {
            seq_t ai           = a_cache[i];
            int   len_ai       = digit_len(ai);
            seq_t pow10ai_mod  = pow10mod((size_t)len_ai, prime);
            seq_t ai_mod       = ai % prime;
            cur_mod = (cur_mod * pow10ai_mod + ai_mod) % prime;
        }

        /* seek the smallest non‑negative integer x with the correct
         * residue modulo 'prime' and with minimal possible digit length
         */
        seq_t target = ((seq_t)(k + 1)) % prime;  /* n mod prime   */

        for (size_t len = 1; ; ++len)
        {
            seq_t pow10len_mod = pow10mod(len, prime);
            seq_t cur_mul      = (cur_mod * pow10len_mod) % prime;
            seq_t r            = (target + prime - cur_mul) % prime;

            seq_t candidate = r;
            /* bump candidate by multiples of 'prime' until it reaches
             * at least the required length                                */
            while (digit_len(candidate) < (int)len)
                candidate += prime;

            /* if the candidate now has the exact length we are done   */
            if (digit_len(candidate) == (int)len)
            {
                a_cache[k] = candidate;
                break;
            }
            /* otherwise try the next length; the sequence guarantees
             * that a solution exists, so this loop must terminate   */
        }
    }

    a_computed = n;
    return a_cache[n - 1];
}
