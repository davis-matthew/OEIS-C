/*
 * A134069  Primes in A038399.
 * https://oeis.org/A134069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  bref, hard, more, nonn, base
 * Formula:   a(n) = nth prime occurring in A038399(n) (concatenation of the
 *            first n non‑zero Fibonacci numbers in reverse order)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    /* static cache of primes found so far */
    static seq_t *prime_cache   = NULL;
    static size_t  cache_cap    = 0;
    static size_t  cache_size   = 0;

    /* state of the concatenation for A038399 */
    static idx_t    cur_k        = 1;          /* current n in A038399 (starts at 1) */
    static seq_t    cur_val      = (seq_t)1;   /* A038399(cur_k) */
    static seq_t    fib_prev    = (seq_t)1;   /* fib(cur_k)   (initially fib(1)) */
    static seq_t    fib_next    = (seq_t)1;   /* fib(cur_k+1) (initially fib(2)) */

    /* initialise cache lazily */
    if (prime_cache == NULL) {
        /* nothing to do – static vars already start with sent values */
    }

    /* generate more terms until we have at least n primes */
    while (cache_size < n) {
        /* build next A038399 term: fib_next * 10^digits(cur_val) + cur_val */
        int    digits = num_digits(cur_val);
        seq_t  p10    = pow10((idx_t)digits);
        seq_t  next_val = fib_next * p10 + cur_val;

        /* test for primality */
        if (is_prime_big(next_val)) {
            /* grow cache if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = mem_new(seq_t, new_cap);
                if (!tmp) return (seq_t)0;                      /* out of memory – unlikely in test harness */
                for (size_t i = 0; i < cache_size; ++i) tmp[i] = prime_cache[i];
                mem_free(prime_cache);
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_size++] = next_val;
        }

        /* advance Fibonacci numbers for next iteration */
        seq_t new_fib = fib_prev + fib_next;
        fib_prev = fib_next;
        fib_next = new_fib;

        /* advance to next A038399 index */
        cur_k++;
        cur_val = next_val;
    }

    /* return the requested prime (1‑based index) */
    return prime_cache[(size_t)n - 1];
}
