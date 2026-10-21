/*
 * A217172  a(n) is the first digit (from the left) to appear seven times in
 *          succession in the decimal representation of n^A217162(n).
 * https://oeis.org/A217172
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the first digit (from the left) to appear seven times in
 *            succession in the decimal representation of n^A217162(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BASE 1000000000U   /* 10^9, each limb holds exactly 9 decimal digits */

/* Simple arbitrary‑precision integer used only for multiplication by a small
   integer and for decimal conversion.  */
typedef struct {
    uint32_t *limbs;   /* little‑endian limbs (limbs[0] is least significant) */
    size_t    len;     /* number of limbs used */
    size_t    cap;     /* allocated capacity (in limbs) */
} BigInt;

/* Initialise a BigInt with a 32‑bit unsigned value.  */
static void bn_init(BigInt *x, uint32_t val)
{
    x->cap = 4;
    x->len = 1;
    x->limbs = (uint32_t *)malloc(x->cap * sizeof(uint32_t));
    if (!x->limbs) exit(1);
    x->limbs[0] = val;
}

/* Free a BigInt.  */
static void bn_free(BigInt *x)
{
    free(x->limbs);
    x->limbs = NULL;
    x->len = x->cap = 0;
}

/* Multiply a BigInt by a small integer m (m < BASE).  */
static void bn_mul_uint32(BigInt *x, uint32_t m)
{
    if (m == 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->limbs[i] * m + carry;
        x->limbs[i] = (uint32_t)(prod % BASE);
        carry       = prod / BASE;
    }
    while (carry) {
        if (x->len == x->cap) {
            size_t newcap = x->cap * 2;
            uint32_t *newlimbs = (uint32_t *)realloc(x->limbs, newcap * sizeof(uint32_t));
            if (!newlimbs) exit(1);
            x->limbs = newlimbs;
            x->cap   = newcap;
        }
        x->limbs[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Return 1 if the decimal representation of x contains a run of seven
   identical digits; the digit is returned through *digit.  */
static int bn_has_seven_run(const BigInt *x, unsigned char *digit)
{
    /* Count digits in the most significant limb. */
    uint32_t msd = x->limbs[x->len - 1];
    unsigned int msd_digits = 0;
    uint32_t tmpd = msd;
    do { ++msd_digits; tmpd /= 10; } while (tmpd);

    /* Total number of decimal digits of the whole integer. */
    size_t total_digits = msd_digits + 9 * (x->len - 1);
    char *s = (char *)malloc(total_digits + 1);
    if (!s) exit(1);

    /* Build the decimal string from most significant limb to least. */
    size_t pos = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        uint32_t limb = x->limbs[i];
        char tmp[32];
        int limb_len = snprintf(tmp, sizeof(tmp), "%u", limb);
        if (i == (int)x->len - 1) {          /* most significant limb */
            for (int j = 0; j < limb_len; ++j)
                s[pos++] = tmp[j];
        } else {                            /* other limbs – pad to 9 digits */
            for (int j = 0; j < 9 - limb_len; ++j)
                s[pos++] = '0';
            for (int j = 0; j < limb_len; ++j)
                s[pos++] = tmp[j];
        }
    }
    s[pos] = '\0';

    /* Scan for the first run of seven identical digits. */
    for (size_t i = 0; i + 7 <= total_digits; ++i) {
        char d = s[i];
        int ok = 1;
        for (int j = 1; j < 7; ++j) {
            if (s[i + j] != d) { ok = 0; break; }
        }
        if (ok) {
            *digit = (unsigned char)(d - '0');
            free(s);
            return 1;
        }
    }

    free(s);
    return 0;
}

/* Main sequence function: a(n).  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* sequence is defined only for n ≥ 2 */

    BigInt bi;
    bn_init(&bi, (uint32_t)n);

    unsigned char digit;
    /* Find the smallest exponent e ≥ 1 such that n^e has a 7‑digit run. */
    while (!bn_has_seven_run(&bi, &digit)) {
        bn_mul_uint32(&bi, (uint32_t)n);
    }

    bn_free(&bi);
    return (seq_t)digit;
}
