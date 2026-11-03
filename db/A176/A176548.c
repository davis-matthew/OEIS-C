/*
 * A176548  Non-semiprimes whose reversal is a prime.
 * https://oeis.org/A176548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Non-semiprimes whose reversal is a prime. No closed formula is
 *            known; we generate the sequence by testing each integer
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem, reverse_digits, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- required utilities ---------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/reverse_digits.h"
#include "../utils/smallest_prime_factor.h"
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

/*--- helper: test whether an unsigned 64‑bit integer is a semiprime -----*/
static bool is_semiprime(uint64_t n)
{
    if (n < 2) return false;
    if (is_prime_seq(n)) return false;          /* prime numbers are not semiprime */
    uint64_t p = smallest_prime_factor(n);
    if (p == 0 || p == n) return false;          /* should not happen for n>=2 */
    uint64_t m = n / p;
    return is_prime_seq(m);                    /* true only if n = p * m with m prime */
}

/*--- cache for the generated terms --------------------------------------*/
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t scan      = 2;   /* next integer to test */

/*--- grow cache if necessary ---------------------------------------------*/
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) abort();                    /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/*--- main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but harmless */

    /* continue generating until we have at least n terms */
    while (cache_len < n) {
        seq_t cand   = (seq_t)scan;            /* candidate value */
        seq_t rev    = reverse_digits(cand);  /* reversal of cand */
        if (is_prime_big(rev)) {
            /* test for non‑semiprimality */
            if (!is_semiprime(scan)) {
                ensure_cache_capacity();
                cache[cache_len++] = cand;
            }
        }
        scan++;   /* move to next integer */
    }

    return cache[n - 1];
}
