/*
 * A010512  Decimal expansion of square root of 59.
 * https://oeis.org/A010512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9837, u16 n <= 9837, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010512 – decimal expansion of √59.
   The algorithm uses the classical digit‑by‑digit method
   (Newton’s algorithm in base 100).  Big integers are represented
   as arrays of 32‑bit limbs in base 1 000 000 000, sufficient for
   the required precision.  All arithmetic is performed on
   these big integers; the function returns only the requested
   decimal digit.  The implementation works for any seq_t type
   (uint8_t … uint64_t or an arbitrary‑precision integer
   class).  The routine preserves state between calls, allowing
   non‑monotonic access to the sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BI_BASE     1000000000u        /* 1e9 base for big ints */
#define BI_BASE_DIG 9                 /* digits per limb */

/* ------------------------------------------------------------------ */
/* Big integer type and helpers                                        */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t *limb;  /* little‑endian limbs, least significant limb first */
    size_t len;      /* number of used limbs (>= 1) */
    size_t cap;      /* allocated capacity */
} bi_t;

static void bi_init(bi_t *b) {
    b->len = 0;
    b->cap = 0;
    b->limb = NULL;
}

static void bi_free(bi_t *b) {
    free(b->limb);
    bi_init(b);
}

static void bi_reserve(bi_t *b, size_t cap) {
    if (cap <= b->cap) return;
    uint32_t *newp = (uint32_t *)realloc(b->limb, cap * sizeof(uint32_t));
    if (!newp) abort();
    b->limb = newp;
    b->cap = cap;
}

static void bi_normalize(bi_t *b) {
    while (b->len > 1 && b->limb[b->len - 1] == 0) b->len--;
}

static void bi_set_uint64(bi_t *b, uint64_t v) {
    bi_reserve(b, 2);
    if (v == 0) {
        b->len = 1;
        b->limb[0] = 0;
    } else {
        b->len = 0;
        while (v) {
            b->limb[b->len++] = (uint32_t)(v % BI_BASE);
            v /= BI_BASE;
        }
    }
}

static void bi_copy(bi_t *dest, const bi_t *src) {
    bi_reserve(dest, src->len);
    memcpy(dest->limb, src->limb, src->len * sizeof(uint32_t));
    dest->len = src->len;
}

static void bi_mul_small(bi_t *b, uint32_t m) {
    if (m == 0) {
        b->len = 1;
        b->limb[0] = 0;
        return;
    }
    bi_reserve(b, b->len + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; i++) {
        uint64_t prod = (uint64_t)b->limb[i] * m + carry;
        b->limb[i] = (uint32_t)(prod % BI_BASE);
        carry = prod / BI_BASE;
    }
    if (carry) {
        b->limb[b->len++] = (uint32_t)carry;
    }
}

static void bi_add_small(bi_t *b, uint32_t add) {
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < b->len) {
        uint64_t sum = (uint64_t)b->limb[i] + carry;
        b->limb[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
        i++;
    }
    if (carry) {
        bi_reserve(b, b->len + 1);
        b->limb[b->len++] = (uint32_t)carry;
    }
}

static void bi_add_bi(bi_t *b, const bi_t *a) {
    size_t maxlen = b->len > a->len ? b->len : a->len;
    bi_reserve(b, maxlen + 1);
    uint64_t carry = 0;
    size_t i = 0;
    for (; i < maxlen; i++) {
        uint64_t sum = carry;
        if (i < b->len) sum += b->limb[i];
        if (i < a->len) sum += a->limb[i];
        b->limb[i] = (uint32_t)(sum % BI_BASE);
        carry = sum / BI_BASE;
    }
    if (carry) {
        b->limb[b->len++] = (uint32_t)carry;
    } else {
        b->len = maxlen;
    }
}

static int bi_compare(const bi_t *a, const bi_t *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i > 0; i--) {
        uint32_t av = a->limb[i - 1];
        uint32_t bv = b->limb[i - 1];
        if (av > bv) return 1;
        if (av < bv) return -1;
    }
    return 0;
}

static void bi_sub(bi_t *b, const bi_t *a) {
    /* assume b >= a */
    uint64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len; i++) {
        uint64_t sub = (uint64_t)b->limb[i] - borrow
                     - (i < a->len ? a->limb[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BI_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        b->limb[i] = (uint32_t)sub;
    }
    bi_normalize(b);
}

/* ------------------------------------------------------------------ */
/* Sequence state (static)                                            */
/* ------------------------------------------------------------------ */
static seq_t *digits = NULL;      /* array of computed digits */
static size_t digits_cap = 0;
static size_t digits_len = 0;

/* Big integer state for the digit‑by‑digit algorithm */
static bi_t root = {0,0,0};
static bi_t rem  = {0,0,0};

/* Temporary big integers reused inside the loop */
static bi_t bi_p     = {0,0,0};
static bi_t bi_tempR = {0,0,0};
static bi_t bi_prod  = {0,0,0};

/* ------------------------------------------------------------------ */
/* Helpers for the digits array                                       */
/* ------------------------------------------------------------------ */
static void ensure_digits_cap(size_t need) {
    if (need <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap*2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    if (!newp) abort();
    digits = newp;
    digits_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Initialization of the algorithm (first digit)                     */
/* ------------------------------------------------------------------ */
static void init_sequence(void) {
    if (digits_len > 0) return;   /* already initialized */

    /* set root = 7 , rem = 59 - 7*7 = 10 */
    bi_set_uint64(&root, 7);
    bi_set_uint64(&rem, 10);

    digits_cap = 8;
    digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
    if (!digits) abort();

    digits[0] = 7;
    digits_len = 1;
}

/* ------------------------------------------------------------------ */
/* Compute digits up to position n (1‑based)                          */
/* ------------------------------------------------------------------ */
static void compute_to(idx_t n) {
    init_sequence();
    for (idx_t cur = digits_len; cur < n; ++cur) {
        /* enough space for the next digit */
        ensure_digits_cap(cur + 1);

        /* p = root * 20 */
        bi_copy(&bi_p, &root);
        bi_mul_small(&bi_p, 20);

        /* tempR = rem * 100 */
        bi_copy(&bi_tempR, &rem);
        bi_mul_small(&bi_tempR, 100);

        /* find largest d in 0..9 s.t. (p + d)*d <= tempR */
        uint32_t d_chosen = 9;
        for (uint32_t d = 9; d <= 9; d--) {   /* loop from 9 down to 0 */
            /* prod = (p + d) * d */
            bi_copy(&bi_prod, &bi_p);
            bi_add_small(&bi_prod, d);
            bi_mul_small(&bi_prod, d);

            if (bi_compare(&bi_prod, &bi_tempR) <= 0) {
                d_chosen = d;
                break;
            }
            if (d == 0) break;
        }

        /* store digit */
        digits[cur] = (seq_t)d_chosen;

        /* update root = root * 10 + d */
        bi_mul_small(&root, 10);
        bi_add_small(&root, d_chosen);

        /* update remainder: rem = tempR - prod */
        bi_copy(&rem, &bi_tempR);          /* rem = tempR */
        bi_sub(&rem, &bi_prod);            /* rem -= prod */
    }
    digits_len = n;
}

/* ------------------------------------------------------------------ */
/* The function required by the exercise                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    compute_to(n);
    return digits[n - 1];
}
