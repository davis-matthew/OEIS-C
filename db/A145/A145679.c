/*
 * A145679  Lower limit of backward value of 2^n and n!.
 * https://oeis.org/A145679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest non‑negative digit a(n) (0–9) such that *
 *            ∑_{i=1..n} a(i)·10^(i‑1) ≡ 0 (mod 2^n). * The sequence starts
 *            with a(1)=2 and for n≥2 the digit is 0 or 1. * Recurrence: let
 *            S_{n‑1}=∑_{i=1..n‑1} a(i)·10^(i‑1). * a(n)=0 if S_{n‑1} ≡ 0 (mod
 *            2^n) * a(n)=1 otherwise (then S_{n‑1} ≡ 2^(n...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  ---  Big integer library (biggest kind of sum we have to handle)  ---  */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_LIMBS 700       /* enough for 10^10000  (≈33220 bits) */

struct bigint {
    uint64_t limb[MAX_LIMBS];
    size_t len;           /* number of used limbs, len==0 → value 0 */
};

/* add b into a (a += b) */
static void bigint_add(struct bigint *a, const struct bigint *b)
{
    size_t i, max = (a->len > b->len) ? a->len : b->len;
    __uint128_t carry = 0;
    for (i = 0; i < max; ++i) {
        __uint128_t av = (i < a->len) ? a->limb[i] : 0;
        __uint128_t bv = (i < b->len) ? b->limb[i] : 0;
        __uint128_t sum = av + bv + carry;
        a->limb[i] = (uint64_t)sum;
        carry = sum >> 64;
    }
    if (carry) {
        if (max < MAX_LIMBS) {
            a->limb[max] = (uint64_t)carry;
            a->len = max + 1;
        }
    } else {
        a->len = max;
    }
}

/* multiply a by a small factor (0…10) */
static void bigint_mul_small(struct bigint *a, uint64_t factor)
{
    if (factor <= 1) {
        if (factor == 0) a->len = 0;
        return;
    }
    __uint128_t carry = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        __uint128_t prod = (__uint128_t)a->limb[i] * factor + carry;
        a->limb[i] = (uint64_t)prod;
        carry = prod >> 64;
    }
    if (carry) {
        if (a->len < MAX_LIMBS) {
            a->limb[a->len] = (uint64_t)carry;
            a->len += 1;
        }
    }
}

/* read bit idx (0‑based) from a big integer */
static inline uint64_t get_bit(const struct bigint *x, size_t idx)
{
    size_t word = idx >> 6;
    if (word >= x->len) return 0;
    return (x->limb[word] >> (idx & 63)) & 1ULL;
}

/*  ---  static state  ---  */
static struct bigint sum;     /* cumulative binary sum */
static struct bigint pow10;   /* current 10^(k-1) value   */
static seq_t *digits = NULL;  /* cached a(i) values       */
static size_t digits_cap = 0; /* capacity of 'digits'     */
static size_t digits_used  = 0; /* number of stored values */

/* helper: grow digits array to at least 'need' elements */
static void grow_digits(size_t need)
{
    if (need <= digits_cap) return;
    size_t new_cap = digits_cap > 0 ? digits_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    if (newptr) {
        digits = newptr;
        digits_cap = new_cap;
    } else {
        /* allocation failed – abort (should not happen in tests) */
        abort();
    }
}

/* initialise static state for first call */
static void init_state(void)
{
    /* start with a(1) = 2 */
    sum.len = 1;
    sum.limb[0] = (uint64_t)2;

    pow10.len = 1;
    pow10.limb[0] = 10ULL;          /* next power of ten to add */

    grow_digits(16);                /* small initial capacity */
    digits[0] = (seq_t)2;
    digits_used = 1;
}

/* ---  public function  ---  */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1 */

    if (digits_used == 0) init_state();

    /* make sure we have space for the requested index */
    grow_digits((size_t)n);

    /* compute any missing terms */
    while (digits_used < (size_t)n) {
        size_t idx = digits_used + 1;          /* 1‑based index to compute */

        /* The value of the (idx‑1)‑th low bit of the current sum
           decides the digit for this index. */
        uint64_t bit = get_bit(&sum, idx - 1);
        seq_t digit = (seq_t)bit;              /* 0 or 1 */

        digits[digits_used] = digit;
        digits_used++;

        /* update sum: S += digit * 10^(idx-1)  (only if digit==1) */
        if (digit) bigint_add(&sum, &pow10);

        /* advance to next power of ten  (10^(idx))   */
        bigint_mul_small(&pow10, 10ULL);
    }

    return digits[n - 1];
}
