/*
 * A050746  Numbers k such that the decimal expansion of k^7 contains no pair
 *          of consecutive equal digits.
 * https://oeis.org/A050746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 2831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050746: numbers k such that the decimal expansion of k^7
   contains no pair of consecutive equal digits.  a(n) is the n‑th
   such number, with a(1) = 0.  The routine searches integers in
   increasing order until the requested index is found. */

/* The implementation uses a small arbitrary‑precision integer
   in base 1 000 000 000 to compute k^7 without overflow, then
   examines its decimal digits.  It works for all supported build
   types (uint8_t … uint128_t, or an arbitrary‑precision class). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

static seq_t *cache      = NULL; /* cached terms        */
static size_t cache_len  = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* array capacity        */
static idx_t  next_k     = 0;    /* next integer to test  */

/* Base for the small big‑integer representation.  1 000 000 000
   fits comfortably into a 32‑bit unsigned integer. */
#define BIGINT_BASE 1000000000u
/* Sufficiently large to hold k^7 for k up to several million. */
#define BIGINT_MAXLEN 16

/* Multiply a small big integer (little‑endian) by an unsigned 32‑bit multiplier. */
static void mul_small(uint32_t *d, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)d[i] * m + carry;
        d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry = prod / BIGINT_BASE;
    }
    while (carry > 0) {
        d[*len] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
        ++*len;
    }
}

/* Divide a small big integer by 10, modifying it in place, and
   return the remainder (the next decimal digit). */
static uint32_t div_mod_10(uint32_t *d, size_t *len)
{
    uint64_t carry = 0;
    size_t i;
    for (i = *len; i > 0; --i) {
        uint64_t cur = carry * BIGINT_BASE + d[i - 1];
        d[i - 1] = (uint32_t)(cur / 10);
        carry = cur % 10;
    }
    /* Remove leading zeros */
    while (*len > 0 && d[*len - 1] == 0) {
        --*len;
    }
    return (uint32_t)carry;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {  /* out of range, return 0 */
        return (seq_t)0;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < (size_t)n) {
        /* Current candidate integer */
        seq_t k = (seq_t)next_k;

        /* Compute k^7 using the small big‑integer helpers. */
        uint32_t big[ BIGINT_MAXLEN ] = {0};
        size_t len = 1;
        big[0] = 1;
        if (k == 0) {
            /* 0^7 == 0 */
            len = 1;
            big[0] = 0;
        } else {
            uint32_t km = (uint32_t)k;
            for (int i = 0; i < 7; ++i) {
                mul_small(big, &len, km);
            }
        }

        /* Check that no two adjacent decimal digits are equal. */
        bool ok = true;
        int prev_digit = -1;
        while (len > 0) {
            int digit = (int)div_mod_10(big, &len);
            if (digit == prev_digit) {
                ok = false;
                break;
            }
            prev_digit = digit;
        }

        if (ok) {
            append_candidate(&cache, &cache_len, &cache_cap, k);
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
