/*
 * A136341  Fibonacci primes or semiprimes F(k) such that F(k+1) is again
 *          prime or semiprime.
 * https://oeis.org/A136341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = F(k) where F(k) is prime or semiprime and F(k+1) is also
 *            prime or semiprime. This function searches Fibonacci numbers
 *            fib(k) and fib(k+1) successively until n terms satisfying the
 *            condition are found, caching results for future calls
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega_seq.h"
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
    /* state kept between calls */
    static seq_t f_prev = (seq_t)1;   /* F(1)  */
    static seq_t f_curr = (seq_t)1;   /* F(2)  */
    static seq_t *terms     = NULL;   /* cached terms */
    static size_t  terms_cap = 0;     /* capacity of terms array */
    static size_t  terms_cnt = 0;     /* number of terms stored */

    /* ensure terms array is allocated */
    if (!terms) {
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
    }

    /* generate terms until we have n of them */
    while (terms_cnt < (size_t)n) {
        /* next Fibonacci number: fib(k+1) = F(k-1)+F(k) */
        seq_t f_next = f_prev + f_curr;

        /* check if F(k)=f_curr is prime or 2-almost prime */
        bool curr_ok = is_prime_big(f_curr);
        if (!curr_ok) {
            curr_ok = (bigomega_seq(f_curr) == (seq_t)2);
        }

        /* check if F(k+1)=f_next is prime or 2-almost prime */
        bool next_ok = is_prime_big(f_next);
        if (!next_ok) {
            next_ok = (bigomega_seq(f_next) == (seq_t)2);
        }

        /* if both are good, store the term */
        if (curr_ok && next_ok) {
            if (terms_cnt >= terms_cap) {
                terms_cap <<= 1;                /* double capacity */
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_cnt++] = f_curr;
        }

        /* advance Fibonacci window: shift left */
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* return the requested term (1‑based index) */
    return terms[n - 1];
}
