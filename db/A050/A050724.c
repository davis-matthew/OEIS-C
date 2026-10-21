/*
 * A050724  Numbers k such that the decimal expansion of 3^k contains no pair
 *          of consecutive equal digits (probably finite).
 * https://oeis.org/A050724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition of OEIS A050724:
   a(n) = the n‑th non‑negative integer k (1‑based) such that
   the decimal expansion of 3^k contains no consecutive equal digits. */

/* Standard C headers */
#include <stdlib.h>
#include <stdbool.h>

/* Global static state – used to allow out‑of‑order calls. */
static idx_t          current_k = 0;      /* exponent of the current power of 3   */
static idx_t *        terms     = NULL;  /* array holding the found k values     */
static size_t        terms_len  = 0;      /* number of terms stored              */
static size_t        terms_cap  = 0;      /* capacity of the terms array          */

static uint8_t *      power     = NULL;  /* little‑endian decimal digits of 3^k   */
static size_t        power_len  = 0;      /* number of used digits                */
static size_t        power_cap  = 0;      /* capacity of the power array           */

/* Ensure the terms array has at least 'needed' capacity. */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    terms = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
    terms_cap = new_cap;
}

/* Ensure the power array has at least 'needed' capacity. */
static void ensure_power_cap(size_t needed)
{
    if (power_cap >= needed) return;
    size_t new_cap = power_cap ? power_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    power = (uint8_t *)realloc(power, new_cap * sizeof(uint8_t));
    power_cap = new_cap;
}

/* Multiply the big integer 'power' by 3. */
static void power_mul3(void)
{
    size_t i;
    int carry = 0;
    for (i = 0; i < power_len; ++i) {
        int tmp = power[i] * 3 + carry;
        power[i] = (uint8_t)(tmp % 10);
        carry    = tmp / 10;
    }
    while (carry > 0) {
        if (power_len == power_cap) ensure_power_cap(power_len + 1);
        power[power_len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* Test whether the current decimal expansion of 3^current_k has
   any pair of consecutive equal digits.  Digits are stored little‑endian. */
static bool no_consecutive_equal_digits(void)
{
    if (power_len <= 1) return true;
    for (size_t i = power_len - 1; i-- > 0; ) {   /* i from len-2 downto 0 */
        if (power[i] == power[i + 1]) return false;
    }
    return true;
}

/* Generate stored terms up to at least the n‑th term. */
static void generate_terms_up_to(size_t n)
{
    /* First‑time initialisation – store k = 0 (3^0 = 1). */
    if (terms_len == 0) {
        ensure_terms_cap(1);
        terms[0] = 0;
        terms_len = 1;

        ensure_power_cap(1);
        power[0] = 1;      /* 3^0 = 1 */
        power_len = 1;
        current_k = 0;
    }

    if (n <= terms_len) return;     /* enough terms already computed */

    ensure_terms_cap(n);

    while (terms_len < n) {
        /* Advance to the next exponent */
        ++current_k;
        power_mul3();

        if (no_consecutive_equal_digits())
            terms[terms_len++] = current_k;
    }
}

/* Function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    generate_terms_up_to((size_t)n);
    return (seq_t)terms[(size_t)(n - 1)];
}
