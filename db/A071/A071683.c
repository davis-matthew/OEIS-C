/*
 * A071683  Nonprimes which are the average of two consecutive Fibonacci
 *          numbers.
 * https://oeis.org/A071683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Generate consecutive Fibonacci numbers.  
   For each pair (F_k,F_{k+1}) compute the integer average  
   (F_k+F_{k+1})/2.  If the average is even and not prime it is  
   stored as the next term of the sequence.  a(1)=1 is the first  
   such non‑prime average. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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
    /* Static cache of already found terms (1‑based index).  */
    static seq_t *cache   = NULL;   /* array of stored values   */
    static size_t cap     = 0;      /* allocated capacity      */
    static size_t used    = 0;      /* number of cached terms   */

    /* Current consecutive Fibonacci pair F_k, F_{k+1}.  */
    static seq_t f = 0, g = 0;      /* f = F_k, g = F_{k+1}    */

    /* Initialise the first Fibonacci pair on the first call. */
    if (used == 0) {
        f = (seq_t)1;  /* F_1 */
        g = (seq_t)1;  /* F_2 */
    }

    /* Extend the cache until it contains at least n terms. */
    while (used < n) {
        seq_t sum = f + g;                 /* F_k + F_{k+1}            */

        /* Keep only even sums, which give integer averages. */
        if ((sum & ((seq_t)1)) == 0) {
            seq_t avg = sum / ((seq_t)2);  /* (F_k + F_{k+1}) / 2      */

            /* Store non‑prime averages (1 is considered nonprime). */
            if (!is_prime_big(avg)) {
                if (used >= cap) {
                    size_t new_cap = cap ? cap << 1 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[used++] = avg;
            }
        }

        /* Advance to the next consecutive Fibonacci numbers. */
        seq_t new_f = g;          /* F_{k+1} becomes new F_k  */
        seq_t new_g = f + g;      /* F_{k+2} = F_k + F_{k+1}  */
        f = new_f;
        g = new_g;
    }

    return cache[(size_t)(n - (idx_t)1)];
}
