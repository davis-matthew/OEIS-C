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
 * Helpers:   _smallest_prime_factor, is_prime_big, is_prime_small,
 *            reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- required utilities ---------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_decimal.h"     /* seq_t reverse_decimal(seq_t v) */
#include "../utils/is_prime_small.h"     /* bool is_prime_small(seq_t num) */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n) */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n) */

/*--- helper: test whether an unsigned 64‑bit integer is a semiprime -----*/
static bool is_semiprime(uint64_t n)
{
    if (n < 2) return false;
    if (is_prime_small(n)) return false;          /* prime numbers are not semiprime */
    uint64_t p = _smallest_prime_factor(n);
    if (p == 0 || p == n) return false;          /* should not happen for n>=2 */
    uint64_t m = n / p;
    return is_prime_small(m);                    /* true only if n = p * m with m prime */
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
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
        seq_t rev    = reverse_decimal(cand);  /* reversal of cand */
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
