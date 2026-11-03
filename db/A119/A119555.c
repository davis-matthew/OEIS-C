/*
 * A119555  Primes in the sequence f(n) = f(n-1)+((-1)^n)*n!, with f(0)=0.
 * https://oeis.org/A119555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the nth prime among the terms f(n)=f(n-1)+((-1)^n)*n!
 *            with f(0)=0, taken only for even indices n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

/* Return the nth prime (1‑based) in the sequence f(n)=f(n-1)+((-1)^n)*n!, f(0)=0.
   The sequence starts with 19 at f(4).  Calls may come in any order; the
   function memoises computed values.  All arithmetic is performed in seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, ignore n==0 */

    /* static cache of primes discovered so far */
    static seq_t *primes      = NULL;
    static size_t primes_cap  = 0;
    static size_t primes_len  = 0;

    /* state of the recurrence: processed up to m_current (m of last f(2m)) */
    static idx_t  m_current   = 0;   /* last m processed */
    static seq_t  f_even_prev = 0;   /* f(2(m-1))            */
    static seq_t  fact_odd    = 0;   /* (2m-1)! of current m */
    static seq_t  fact_even   = 0;   /* (2m)!   of current m */

    /* Initialise once.  f(0)=0, 1!=1, 2!=2. */
    if (m_current == 0) {
        f_even_prev = 0;
        fact_odd    = (seq_t)1;
        fact_even   = (seq_t)2;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        /* Next even index: m = m_current+1, n_even = 2*m */
        ++m_current;   /* now m >= 1 */

        if (m_current == 1) {
            /* fact_odd and fact_even already set to 1! and 2! */
        } else {
            /* Use previous fact_even to build new odd and even factorials. */
            seq_t prev_even = fact_even;
            seq_t mult_odd  = (seq_t)(2 * m_current - 1);
            seq_t mult_even = (seq_t)(2 * m_current);
            fact_odd  = prev_even * mult_odd;
            fact_even = fact_odd * mult_even;
        }

        /* f(2m) = f(2(m-1)) + (2m)! – (2m-1)! */
        seq_t f_even = f_even_prev + (fact_even - fact_odd);

        /* If the value is prime, cache it. */
        if (f_even > (seq_t)1 && is_prime_big(f_even)) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) return (seq_t)0;  /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = f_even;
        }

        /* Prepare for next iteration. */
        f_even_prev = f_even;
    }

    return primes[n - 1];
}
