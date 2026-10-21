/*
 * A227871  Sum of digits of 14^n.
 * https://oeis.org/A227871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of digits of 14^n   (OEIS A227871) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Base for the internal big‑integer representation (fits into uint32_t) */
static const uint32_t BASE = 1000000000U;

/* --------------------------------------------------------------------- */
/*  State ---------------------------------------------------------------- */
static idx_t   cached_max = 0;          /* largest n already computed   */
static size_t  pow_cap    = 0;          /* allocated size of pow_limbs   */
static uint32_t *pow_limbs = NULL;     /* limbs of 14^cached_max         */
static size_t  pow_len    = 0;          /* number of limbs actually used */

static size_t  res_cap = 0;            /* allocated size of result array */
static seq_t  *results  = NULL;        /* a(0)…a(cached_max)            */

/* --------------------------------------------------------------------- */
/*  Helper: table that gives the digit sum of numbers 0…9999 */
static uint8_t  digit_sum_4[10000];
static bool digit_sum_inited = false;

static void
init_digit_sum(void)
{
    if (!digit_sum_inited) {
        for (int i = 0; i < 10000; ++i) {
            int x = i, s = 0;
            while (x > 0) { s += x % 10; x /= 10; }
            digit_sum_4[i] = (uint8_t)s;
        }
        digit_sum_inited = true;
    }
}

static seq_t
digit_sum_limbs(const uint32_t *limbs, size_t len)
{
    init_digit_sum();
    uint64_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t v = limbs[i];
        do {
            sum += digit_sum_4[v % 10000];
            v /= 10000;
        } while (v > 0);
    }
    return (seq_t)sum;
}

/* --------------------------------------------------------------------- */
/*  Ensure the big‑integer limb array is large enough (C++ cast) */
static void
ensure_pow_capacity(size_t needed)
{
    if (pow_cap < needed) {
        size_t new_cap = pow_cap ? pow_cap << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        uint32_t *tmp = (uint32_t*)realloc(pow_limbs,
                                        new_cap * sizeof(uint32_t));
        if (!tmp) abort();          /* out of memory */
        pow_limbs = tmp;
        pow_cap   = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Ensure the result array has room (C++ cast) */
static void
ensure_res_capacity(idx_t n)
{
    if ((size_t)(n + 1) > res_cap) {
        size_t new_cap = res_cap ? res_cap << 1 : 64;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(results,
                                    new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        results = tmp;
        res_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Initialise the state to 14^0 */
static void
init_zero(void)
{
    if (!pow_limbs) {
        ensure_pow_capacity(1);
        pow_limbs[0] = 1;
        pow_len     = 1;
        cached_max  = 0;
        ensure_res_capacity(0);
        results[0]  = (seq_t)1;            /* digit sum of 1 is 1 */
    }
}

/* --------------------------------------------------------------------- */
/*  Public entry point -------------------------------------------------- */
seq_t a(idx_t n)
{
    init_zero();

    if (n <= cached_max)
        return results[n];

    /* Compute powers from cached_max+1 up to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        /* multiply current big‑integer by 14 */
        uint64_t carry = 0;
        for (size_t i = 0; i < pow_len; ++i) {
            uint64_t prod = (uint64_t)pow_limbs[i] * 14 + carry;
            pow_limbs[i] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        if (carry) {
            ensure_pow_capacity(pow_len + 1);
            pow_limbs[pow_len] = (uint32_t)carry;
            ++pow_len;
        }

        /* store digit sum */
        ensure_res_capacity(k);
        results[k] = digit_sum_limbs(pow_limbs, pow_len);
        cached_max = k;
    }
    return results[n];
}
