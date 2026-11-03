/*
 * A176111  Honaker primes of the form p = 2*k-1 with sum-of-digits(p) =
 *          sum-of-digits(k).
 * https://oeis.org/A176111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The function enumerates Honaker primes that also satisfy the extra
   condition p = 2*k-1 with digit_sum(p) = digit_sum(k).  Let p be a prime,
   let j be its index in the ordered list of primes (so p = prime(j)),
   and let k = (p+1)/2.  The sequence term is p when
   digit_sum(p) == digit_sum(j) == digit_sum(k).

   The implementation expands the sequence incrementally, caching found
   terms so that subsequent calls with any index are answered in
   amortised O(1) time after the needed terms have been generated.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

/* cache of previously found terms */
static seq_t *term_cache = NULL;
static idx_t term_len   = 0;
static idx_t term_cap   = 0;

/* state for incremental prime generation */
static idx_t prime_index   = 0;    /* number of primes seen so far */
static idx_t candidate    = 2;    /* next integer to test for primality */

/* append a new term to the cache */
static void append_term(seq_t v)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? term_cap << 1 : 16;
        seq_t *tmp = mem_resize(seq_t, term_cache, term_cap);
        if (!tmp) abort();
        term_cache = tmp;
    }
    term_cache[term_len++] = v;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0 */

    /* Generate terms until we have at least n of them */
    while (term_len < n) {
        seq_t p = (seq_t)candidate;

        if (is_prime_big(p)) {
            prime_index++;                 /* p is the prime_index‑th prime */
            idx_t j = prime_index;         /* index j of p */

            /* compute k = (p+1)/2 */
            seq_t k = (p + (seq_t)1) / (seq_t)2;

            /* digit sums */
            seq_t ds_p = digit_sum(p);
            seq_t ds_j = digit_sum((seq_t)j);
            seq_t ds_k = digit_sum(k);

            if (ds_p == ds_j && ds_p == ds_k) {
                append_term(p);
            }
        }

        /* advance candidate */
        if (candidate == 2) {
            candidate = 3;   /* next odd number after 2 */
        } else {
            candidate += 2;   /* skip even numbers */
        }
    }

    return term_cache[n - 1];
}
