/*
 * A063584  Smallest k such that 4^k has exactly n 9's in its decimal
 *          representation.
 * https://oeis.org/A063584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A063584
 *
 * a(n) = the smallest k such that the decimal representation of 4^k
 * has exactly n digits equal to '9'.
 * Offset 0: a(0) = 1.
 * The implementation keeps a decimal array for 4^k, multiplies by 4
 * on each step, updates the count of '9' digits, and stores the first
 * k that achieves each new count.  Results are cached so calls can
 * arrive in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Decimal representation of 4^k (least‑significant digit first) ---------- */
static uint8_t *digits      = NULL;   /* array of decimal digits */
static idx_t    digits_len  = 0;      /* amount of digits used       */
static idx_t    digits_cap  = 0;      /* allocated capacity          */

/* Ensure space for at least new_cap digits. */
static void ensure_digits_capacity(idx_t new_cap)
{
    if (digits_cap >= new_cap) return;
    idx_t cap = digits_cap ? digits_cap : 16;
    while (cap < new_cap) cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
    if (!tmp) exit(1);          /* fatal allocation error */
    digits     = tmp;
    digits_cap = cap;
}

/* Initialize to 4^1 = 4. */
static void digits_init(void)
{
    ensure_digits_capacity(64);
    digits[0] = 4;
    digits_len = 1;
}

/* ---------- Cache for the sequence a(n) ---------- */
static uint64_t *a_cache  = NULL;   /* a_cache[n] = smallest k for n 9's (0 means unknown) */
static idx_t    a_cap    = 0;       /* allocated size of a_cache */

/* Ensure room for indices up to needed (inclusive).  New entries are zero‐filled. */
static void ensure_a_capacity(idx_t needed)
{
    if (a_cap > needed) return;
    idx_t cap = a_cap ? a_cap : 8;
    while (cap <= needed) cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(a_cache, cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    for (idx_t i = a_cap; i < cap; ++i) tmp[i] = 0;
    a_cache = tmp;
    a_cap   = cap;
}

/* ---------- Incremental state ---------- */
static bool      initialized   = false;
static uint64_t  current_k     = 1;   /* exponent of current power of 4 */
static idx_t     nine_count    = 0;   /* number of '9' digits in current 4^k */

/* Multiply the current decimal number by 4, updating the digit array
   and the nine_count. */
static void multiply_by_4(void)
{
    idx_t i;
    int carry = 0;
    for (i = 0; i < digits_len; ++i) {
        int old = digits[i];
        int prod = old * 4 + carry;
        int newd = prod % 10;
        carry   = prod / 10;
        if (old == 9) --nine_count;
        if (newd == 9) ++nine_count;
        digits[i] = (uint8_t)newd;
    }
    while (carry > 0) {
        int newd = carry % 10;
        carry   /= 10;
        if (digits_len == digits_cap) ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = (uint8_t)newd;
        if (newd == 9) ++nine_count;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ----- initialisation on first call ----- */
    if (!initialized) {
        digits_init();                 /* 4^1 = 4          */
        ensure_a_capacity(16);         /* room for some early values */
        a_cache[0] = 1;                /* a(0)=1 (0 9's)   */
        initialized = true;
    }

    /* ----- cached result? ----- */
    if (n < a_cap && a_cache[n] != 0)
        return (seq_t)a_cache[n];

    /* ----- generate successive powers of 4 until a(n) is known ----- */
    while (true) {
        multiply_by_4();           /* advance to 4^(current_k+1) */
        ++current_k;

        /* Record this k for the new nine_count if not already stored. */
        if (nine_count >= a_cap)
            ensure_a_capacity(nine_count);
        if (a_cache[nine_count] == 0)
            a_cache[nine_count] = current_k;

        /* If we now know a(n), return it. */
        if (n < a_cap && a_cache[n] != 0)
            return (seq_t)a_cache[n];
    }
}
