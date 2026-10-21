/*
 * A051271  Number of numbers either relatively prime to or divisors of
 *          primorial number(n).
 * https://oeis.org/A051271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + ∏_{i=1..n} (p_i – 1) (p_i = i‑th prime)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of numbers either relatively prime to or divisors of primorial number(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------ */
/*   static data shared between calls                                      */
/* ------------------------------------------------------------------------ */

static uint64_t *primes      = NULL;   /* primes[0 .. prime_cnt-1]   */
static size_t   prime_cnt    = 0;     /* number of primes known     */
static size_t   prime_cap    = 0;     /* capacity of primes array   */

static seq_t   *values       = NULL;   /* memoized a(1 .. max_n)    */
static idx_t    max_n        = 0;      /* largest n computed       */
static size_t   values_cap   = 0;      /* capacity of values array  */

static seq_t   pow2   = 1;             /* 2^current_n               */
static seq_t   prod   = 1;             /* ∏_{i=1..current_n} (p_i-1) */

/* ------------------------------------------------------------------------ */
/*   generate at least 'needed' primes (ascending)                          */
/* ------------------------------------------------------------------------ */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed) return;

    while (prime_cnt < needed) {
        uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

        /* find the next prime */
        bool is_prime = false;
        while (!is_prime) {
            is_prime = true;
            for (size_t k = 0; k < prime_cnt; ++k) {
                uint64_t p = primes[k];
                if (p > cand / p) break;            /* p*p > cand */
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (!is_prime) cand++;    /* try next number */
        }

        /* store the discovered prime */
        if (prime_cnt == prime_cap) {
            size_t new_cap = (prime_cap == 0) ? 4 : prime_cap * 2;
            uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
            prime_cap = new_cap;
        }
        primes[prime_cnt++] = cand;
    }
}

/* ------------------------------------------------------------------------ */
/*   compute a(1 .. n) and store them in the values array                 */
/* ------------------------------------------------------------------------ */
static void ensure_values(idx_t n)
{
    if (n <= max_n) return;

    /* make sure we have the required primes */
    ensure_primes(n);

    /* grow the values array if necessary */
    if (n + 1 > values_cap) {
        size_t new_cap = (values_cap == 0) ? 16 : values_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        values = tmp;
        values_cap = new_cap;
    }

    /* compute and store new terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        pow2 = pow2 * (seq_t)2;
        prod = prod * (seq_t)(primes[i - 1] - 1);
        values[i] = pow2 + prod;
    }
    max_n = n;
}

/* ------------------------------------------------------------------------ */
/*   public sequence function                                            */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1, ignored per spec */
    ensure_values(n);
    return values[n];
}
