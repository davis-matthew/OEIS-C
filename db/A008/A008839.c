/*
 * A008839  Numbers k such that the decimal expansion of 5^k contains no
 *          zeros.
 * https://oeis.org/A008839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = exponent k such that 5^k has no zero digits in its
 *            decimal expansion. The function searches successive powers of 5
 *            and records the exponents that satisfy the zero‑digit test. It
 *            maintains a cache of already found terms so that repeated calls
 *            with any order of n succeed quickly. The digits...
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- internal state ---------- */
static unsigned char *digits = NULL;      /* decimal digits of current 5^k (least
                                            significant digit first)          */
static size_t digits_len = 0, digits_cap = 0;  /* length & capacity of digits */

static seq_t *cache = NULL;              /* cache of exponent values k          */
static size_t cache_len = 0, cache_cap = 0;   /* length & capacity of cache        */

static idx_t next_k = 0;                 /* exponent currently represented in
                                            'digits'                            */

/* ---------- helpers ---------- */
static void ensure_digits_cap(size_t needed)
{
    if (digits_cap >= needed) return;
    size_t new_cap = (digits_cap == 0) ? 8 : digits_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(digits, new_cap);
    if (!tmp) abort();                      /* fatal if memory allocation fails   */
    digits = tmp;
    digits_cap = new_cap;
}

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void init_state(void)
{
    /* initialise the first power 5^0 = 1 */
    ensure_digits_cap(16);
    digits[0] = 1;
    digits_len = 1;

    /* initialise cache */
    ensure_cache_cap(8);
    next_k = 0;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* Lazily initialise on first call */
    if (digits == NULL) init_state();

    if (n == 0) return (seq_t)0;   /* undefined – safety return */

    /* Ensure we have at least n terms cached */
    while (cache_len < (size_t)n)
    {
        /* Check whether current 5^next_k contains a zero digit. */
        bool has_zero = false;
        for (size_t i = 0; i < digits_len; ++i)
        {
            if (digits[i] == 0)
            {
                has_zero = true;
                break;
            }
        }

        if (!has_zero)
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }

        /* Multiply current 5^next_k by 5 to get next power. */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i)
        {
            unsigned int prod = digits[i] * 5 + carry;
            digits[i] = (unsigned char)(prod % 10);
            carry    = prod / 10;
        }
        while (carry > 0)
        {
            ensure_digits_cap(digits_len + 1);
            digits[digits_len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }

        ++next_k;   /* advance to next exponent */
    }

    return cache[n - 1];
}
