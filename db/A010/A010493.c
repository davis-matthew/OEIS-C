/*
 * A010493  Decimal expansion of square root of 39.
 * https://oeis.org/A010493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9776, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the decimal expansion of √39 digit by digit.
   The digit at position n (n ≥ 1) is returned.
   Uses a digit‑by‑digit algorithm with a small‑base big‑integer
   representation that never overflows 64‑bit types.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000U            /* 10^9 base for big integers */

/* ----- small big‑integer type (array of uint32_t limbs) ----- */
typedef struct {
    uint32_t *limb;   /* little‑endian limbs, limb[0] least significant */
    size_t sz;        /* number of used limbs */
    size_t cap;       /* allocated capacity */
} bi_t;

/* Create a big integer with given capacity. */
static bi_t bi_new(size_t cap) {
    bi_t r;
    r.limb = (uint32_t*)calloc(cap, sizeof(uint32_t));
    r.sz = 0;
    r.cap = cap;
    return r;
}

/* Ensure capacity. */
static void bi_ensure_cap(bi_t *b, size_t cap) {
    if (b->cap < cap) {
        size_t newcap = cap;
        while (newcap < cap) newcap *= 2;
        uint32_t *newlimb = (uint32_t*)realloc(b->limb, newcap * sizeof(uint32_t));
        if (!newlimb) abort();
        memset(newlimb + b->cap, 0, (newcap - b->cap) * sizeof(uint32_t));
        b->limb = newlimb;
        b->cap = newcap;
    }
}

/* Trim leading zeros. */
static void bi_trim(bi_t *b) {
    while (b->sz > 0 && b->limb[b->sz-1] == 0) --b->sz;
}

/* Set to an unsigned integer. */
static void bi_set_uint(bi_t *b, uint32_t val) {
    b->sz = 0;
    if (val) {
        bi_ensure_cap(b, 1);
        b->limb[0] = val;
        b->sz = 1;
    }
}

/* Copy a big integer. */
static void bi_copy(bi_t *dst, const bi_t *src) {
    bi_ensure_cap(dst, src->sz);
    memcpy(dst->limb, src->limb, src->sz * sizeof(uint32_t));
    dst->sz = src->sz;
}

/* Add small integer to a big integer, result in dst (can be same as src). */
static void bi_add_small(bi_t *dst, const bi_t *src, uint32_t add) {
    bi_ensure_cap(dst, src->sz + 1);
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < src->sz; ++i) {
        uint64_t sum = (uint64_t)src->limb[i] + carry;
        dst->limb[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        dst->limb[i++] = (uint32_t)carry;
    }
    dst->sz = i;
    bi_trim(dst);
}

/* Multiply a big integer by small integer, result in dst. */
static void bi_mul_small(bi_t *dst, const bi_t *src, uint32_t mul) {
    if (mul == 0 || src->sz == 0) { dst->sz = 0; return; }
    bi_ensure_cap(dst, src->sz + 1);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < src->sz; ++i) {
        uint64_t prod = (uint64_t)src->limb[i] * mul + carry;
        dst->limb[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        dst->limb[i++] = (uint32_t)carry;
    }
    dst->sz = i;
    bi_trim(dst);
}

/* Subtract b from a (a >= b), result in dst. */
static void bi_sub(bi_t *dst, const bi_t *a, const bi_t *b) {
    bi_ensure_cap(dst, a->sz);
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->sz; ++i) {
        int64_t ai = a->limb[i];
        int64_t bi = (i < b->sz) ? b->limb[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        dst->limb[i] = (uint32_t)diff;
    }
    dst->sz = a->sz;
    bi_trim(dst);
}

/* Compare two big integers, return -1,0,1 as usual. */
static int bi_cmp(const bi_t *a, const bi_t *b) {
    if (a->sz != b->sz) return (a->sz > b->sz) ? 1 : -1;
    for (size_t i = a->sz; i-- > 0;) {
        if (a->limb[i] != b->limb[i]) return (a->limb[i] > b->limb[i]) ? 1 : -1;
    }
    return 0;
}

/* ----- state for the sequence ----- */
static bi_t ROOT      = {0,0,0};
static bi_t REM       = {0,0,0};
static bi_t TMP       = {0,0,0};
static bi_t COMP      = {0,0,0};
static bi_t PROD      = {0,0,0};
static bi_t TMP2      = {0,0,0};
static bi_t TMP3      = {0,0,0};

static seq_t *DIGITS = NULL;      /* array of computed decimal digits */
static size_t DIGITS_CAP = 0;
static idx_t  DIGITS_LEN = 0;

/* Ensure enough capacity for DIGITS array. */
static void ensure_digit_capacity(size_t needed) {
    if (DIGITS_CAP >= needed) return;
    size_t newcap = (DIGITS_CAP > 0) ? DIGITS_CAP * 2 : 8;
    while (newcap < needed) newcap *= 2;
    seq_t *newptr = (seq_t*)realloc(DIGITS, newcap * sizeof(seq_t));
    if (!newptr) abort();
    DIGITS = newptr;
    DIGITS_CAP = newcap;
}

/* Main function requested. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initialize big integer buffers on first call. */
    static int initialized = 0;
    if (!initialized) {
        /* preallocate with generous capacity */
        ROOT  = bi_new(4096);
        REM   = bi_new(4096);
        TMP   = bi_new(4096);
        COMP  = bi_new(4096);
        PROD  = bi_new(4096);
        TMP2  = bi_new(4096);
        TMP3  = bi_new(4096);
        initialized = 1;
    }

    /* Compute digits up to n if not yet done. */
    if (n > DIGITS_LEN) {
        ensure_digit_capacity((size_t)n);
        for (idx_t k = DIGITS_LEN; k < n; ++k) {   /* k is 0‑based index of next digit to compute */
            /* Pair for this step */
            uint32_t pair = (k == 0) ? 39U : 0U;

            /* COMP = REM * 100 + pair */
            bi_mul_small(&COMP, &REM, 100U);
            bi_add_small(&COMP, &COMP, pair);

            /* TMP = ROOT * 20 */
            bi_mul_small(&TMP, &ROOT, 20U);

            /* Find largest digit d such that (TMP + d) * d <= COMP */
            uint32_t digit = 0;
            for (uint32_t cand = 9; cand <= 9; --cand) { /* loop 9 .. 0 */
                /* TMP2 = TMP + cand */
                bi_add_small(&TMP2, &TMP, cand);
                /* PROD = TMP2 * cand */
                bi_mul_small(&PROD, &TMP2, cand);
                if (bi_cmp(&PROD, &COMP) <= 0) {
                    digit = cand;
                    break;
                }
                if (cand == 0) break;          /* avoid infinite loop on unsigned */
            }

            /* REM = COMP - PROD */
            bi_sub(&REM, &COMP, &PROD);

            /* ROOT = ROOT * 10 + digit */
            bi_mul_small(&TMP, &ROOT, 10U);     /* reuse TMP as scratch */
            bi_add_small(&TMP, &TMP, digit);
            bi_copy(&ROOT, &TMP);

            /* Store digit */
            DIGITS[k] = (seq_t)digit;
        }
        DIGITS_LEN = n;
    }

    return DIGITS[n-1];
}
