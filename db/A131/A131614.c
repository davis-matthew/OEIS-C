/*
 * A131614  Numbers k such that the decimal expansion of 3^k contains no 8.
 * https://oeis.org/A131614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the decimal expansion of 3^k contains no digit 8. */
/* Computed by iteratively multiplying a big integer by 3 and testing its
   decimal digits for the presence of the digit 8. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BIGBASE (1000000000U)   /* Base 10^9 for the big integer limbs */

/* ------------------------------------------------------------------ */
/* Simple arbitrary‑precision integer – only the operations that we
   need are provided.                                       */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t *limb;   /* least significant limb first */
    size_t   len;     /* number of limbs used */
} BigInt;

static void big_init(BigInt *b)
{
    b->len   = 1;
    b->limb  = (uint32_t*)malloc(sizeof(uint32_t));
    if (!b->limb) abort();
    b->limb[0] = 1;          /* start with value 1 (3^0) */
}

static void big_mul3(BigInt *b)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limb[i] * 3 + carry;
        b->limb[i] = (uint32_t)(prod % BIGBASE);
        carry      = prod / BIGBASE;
    }
    if (carry) {
        b->limb = (uint32_t*)realloc(b->limb, (b->len + 1) * sizeof(uint32_t));
        if (!b->limb) abort();
        b->limb[b->len] = (uint32_t)carry;
        ++b->len;
    }
}

/* Return true if the integer represented by BigInt has no decimal digit 8. */
static bool big_has_no_eight(const BigInt *b)
{
    for (size_t i = 0; i < b->len; ++i) {
        uint32_t limb = b->limb[i];
        for (int d = 0; d < 9; ++d) {   /* each limb has at most 9 decimal digits */
            if (limb % 10U == 8U) return false;
            limb /= 10U;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Memoised sequence storage and the main function.                  */
/* ------------------------------------------------------------------ */
static idx_t   *found   = NULL;   /* array of exponents k satisfying the condition */
static size_t   found_cap = 0;
static size_t   found_len = 0;

static idx_t   next_k  = 0;   /* exponent of the current power of 3 */
static BigInt  big;          /* current value of 3^next_k */

static void append_k(idx_t k)
{
    if (found_len == found_cap) {
        size_t new_cap = found_cap ? found_cap * 2 : 8;
        idx_t *tmp = (idx_t*)realloc(found, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        found  = tmp;
        found_cap = new_cap;
    }
    found[found_len++] = k;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined for n ≥ 1 only */

    /* Initialise on first call */
    static bool init_done = false;
    if (!init_done) {
        big_init(&big);
        init_done = true;
    }

    /* Fill the sequence until we have at least n terms */
    while (found_len < n) {
        if (big_has_no_eight(&big))
            append_k(next_k);
        big_mul3(&big);
        ++next_k;
    }

    return (seq_t)found[n - 1];
}
