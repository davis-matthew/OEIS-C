/*
 * A214752  Primes in the sequence of tribonacci numbers starting 1, 1, 0
 *          (A081172).
 * https://oeis.org/A214752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime among the Tribonacci numbers with initial
 *            terms T(1)=1, T(2)=1, T(3)=0 and recurrence
 *            T(k)=T(k-1)+T(k-2)+T(k-3). (The sequence of such primes is
 *            A214752 in OEIS.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* static storage for the list of primes found so far and
       the current Tribonacci window.  All variables are initialised
       once for the lifetime of the program. */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t t1 = 1, t2 = 1, t3 = 0;

    /* Generate new Tribonacci numbers until we have at least n primes. */
    while (primes_cnt < (size_t)n)
    {
        /* next T(k) = T(k-1)+T(k-2)+T(k-3) */
        seq_t next = t1 + t2 + t3;

        /* shift window for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = next;

        /* If the new Tribonacci number is prime, store it. */
        if (is_prime_big(t3))
        {
            if (primes_cnt == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                void *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp)
                    abort();          /* out of memory */
                primes = (seq_t*)tmp; /* cast for C and C++ */
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = t3;
        }
    }

    /* Return the requested prime (n is 1‑based). */
    return primes[n - 1];
}
