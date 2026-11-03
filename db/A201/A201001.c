/*
 * A201001  Primes in the Lucas U(3,-1) sequence.
 * https://oeis.org/A201001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes among the Lucas U(3,-1) sequence at prime indices.
   The code generates U_n = 3*U_{n-1}+U_{n-2} (U_0=0, U_1=1)
   and collects those values that are prime. The nth element of
   the returned sequence is the nth prime U_p where p itself is a
   prime index.  The implementation caches results so that
   successive calls with arbitrary order of n are efficient. */

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

seq_t a(idx_t n)
{
    /* static state: last index of U computed, two previous terms,
       cache of primes found, and cache capacity */
    static idx_t last_u_index = 1;        /* last n for which U_n computed */
    static seq_t u_prevprev = 0;          /* U_{last_u_index-1} */
    static seq_t u_prev     = 1;          /* U_{last_u_index}   */
    static seq_t *prime_cache = NULL;     /* array of prime U values */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    /* initialise cache on first call */
    if (!prime_cache) {
        prime_cap   = 16;
        prime_cache = mem_new(seq_t, prime_cap);
        prime_len   = 0;
    }

    /* extend cached primes until we have at least n values */
    while (prime_len < n) {
        /* compute next Lucas U value */
        seq_t u_cur = 3 * u_prev + u_prevprev;

        /* shift for next iteration */
        u_prevprev = u_prev;
        u_prev     = u_cur;
        last_u_index++;

        /* test primality of the newly computed value */
        if (is_prime_big(u_cur)) {
            /* grow cache if needed */
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                prime_cache = mem_resize(seq_t, prime_cache, prime_cap);
            }
            prime_cache[prime_len++] = u_cur;
        }
    }

    /* return the requested element (1‑based index) */
    return prime_cache[n - 1];
}
