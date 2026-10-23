/*
 * A045552  Numbers whose factorial has '08' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A045552
 *  Numbers m such that m! ends with the digits …0800… (i.e. the two
 *  digits immediately before the trailing zeros are 08).
 *
 *  The algorithm maintains
 *
 *      prod   – product of all factors of m! with every 2 and 5 removed,
 *               taken modulo 100;
 *      v2     – exponent of 2 in m!;
 *      v5     – exponent of 5 in m!;
 *
 *  For each new m it updates prod, v2, v5, then computes
 *
 *      r = prod × 2^(v2−v5)  (mod 100)
 *
 *  If r == 8 the two digits just before the trailing zeros are 08,
 *  so m is a term of the sequence.
 *
 *  A small static cache is maintained so that calls with arbitrary
 *  n are answered in O(number of required terms) time, independent
 *  of the order of the calls.
 *
 *  Only unsigned arithmetic (no floating‑point) and the standard
 *  integer types provided by the build are used.
 */

#include <stdint.h>
#include <stdlib.h>

#define MODULO 100U

/* fast exponentiation of 2 modulo 100 */
static uint8_t pow2_mod_uint64(uint64_t exp)
{
    uint8_t result = 1U;
    uint8_t base   = 2U;

    while (exp != 0) {
        if (exp & 1ULL)
            result = (uint8_t)((result * base) % MODULO);
        base = (uint8_t)((base * base) % MODULO);
        exp >>= 1ULL;
    }
    return result;
}

static uint8_t prod = 1U;          /* product modulo 100 of all factors with 2s/5s removed */
static uint64_t v2 = 0;            /* exponent of 2 in the accumulated factorial */
static uint64_t v5 = 0;            /* exponent of 5 in the accumulated factorial */
static uint64_t iter = 0;          /* current m being examined (last tested value) */

static seq_t *cache = NULL;        /* cache of found terms           */
static size_t cache_size = 0;      /* number of terms currently stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache    */

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* If allocation fails we cannot recover; terminate. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1; behaviour for n==0 is not significant */

    /* grow cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        iter++;                    /* next integer to test */

        uint64_t t = iter;
        uint8_t  c2 = 0, c5 = 0;

        /* count factors of 2 */
        while ((t & 1ULL) == 0) {
            t >>= 1;
            ++c2;
        }
        v2 += c2;

        /* count factors of 5 */
        while (t % 5ULL == 0) {
            t /= 5ULL;
            ++c5;
        }
        v5 += c5;

        /* multiply remaining factor (coprime to 10) into prod modulo 100 */
        prod = (uint8_t)((prod * (uint8_t)(t % MODULO)) % MODULO);

        /* factorial without trailing zeros modulo 100 */
        uint64_t exp = v2 - v5;    /* v2 >= v5 for factorial */
        uint8_t r = (uint8_t)((prod * pow2_mod_uint64(exp)) % MODULO);

        if (r == 8) {              /* last two digits are 08 before trailing zeros */
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)iter;
        }
    }

    return cache[n - 1];
}
