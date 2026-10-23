/*
 * A035140  Digits of juxtaposition of prime factors of composite n appear
 *          also in n.
 * https://oeis.org/A035140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth composite such that every decimal digit appearing
   in the concatenated list of its prime factors also appears in n. */

/* --------------------------------------------------------------------- */
/* Included headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Compute a bitmask of decimal digits of a 64‑bit unsigned integer */
static inline uint16_t digit_mask_uint64(uint64_t v)
{
    uint16_t mask = 0;
    if (v == 0) return 1;               /* only digit 0 */
    while (v) {
        mask |= 1u << (v % 10u);
        v /= 10u;
    }
    return mask;
}

/* --------------------------------------------------------------------- */
/* Smallest prime factor sieve */
/* spf[i] == smallest prime factor of i (spf[prime] == prime) */
/* Empty (NULL) means not yet initialized. */
static uint32_t *spf = NULL;
static size_t   spf_limit = 0;

/* Ensure that spf is computed up to at least “limit”.  The array is
   allocated and, if necessary, enlarged to a power‑of‑two size. */
static void ensure_spf(uint64_t limit)
{
    if (limit <= spf_limit) return;

    size_t new_limit = spf_limit ? spf_limit : 100;
    while (new_limit < limit) new_limit <<= 1;   /* double */

    uint32_t *new_spf =
        (uint32_t *)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(1);
    spf = new_spf;

    /* Initialize spf[i] = i for all new entries */
    for (size_t i = spf_limit + 1; i <= new_limit; ++i) spf[i] = (uint32_t)i;

    /* Sieve only over the new part if spf_limit > 1 */
    if (new_limit >= 2) {
        for (size_t i = 2; i <= new_limit / i; ++i) {
            if (spf[i] == (uint32_t)i) {
                for (size_t j = i * i; j <= new_limit; j += i) {
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/* Test whether n (assumed composite) satisfies the digit condition. */
static bool satisfies(uint64_t n)
{
    ensure_spf(n);

    /* Must be composite. */
    if (spf[(size_t)n] == (uint32_t)n) return false;

    uint16_t mask_n     = digit_mask_uint64(n);
    uint16_t mask_facs  = 0;

    uint32_t temp = (uint32_t)n;
    while (temp > 1) {
        uint32_t f = spf[(size_t)temp];
        mask_facs |= digit_mask_uint64(f);
        while (temp % f == 0) temp /= f;
    }
    return (mask_facs & ~mask_n) == 0;
}

/* --------------------------------------------------------------------- */
/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 (offset 1) */

    uint64_t count = 0;
    uint64_t candidate = 4;           /* first composite */

    while (true) {
        if (satisfies(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;   /* cast works for both C and C++ */
            }
        }
        ++candidate;
    }
}
