/*
 * A093785  Numbers that are divisible by every digit in their Roman numeral
 *          representation.
 * https://oeis.org/A093785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth integer that is divisible by every Roman *
 *            numeral symbol that appears in its Roman representation. * The
 *            sequence contains only numbers ≤ 3999
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Roman numeral symbols and their numeric values */
#define S_I (1 << 0)
#define S_V (1 << 1)
#define S_X (1 << 2)
#define S_L (1 << 3)
#define S_C (1 << 4)
#define S_D (1 << 5)
#define S_M (1 << 6)

static const seq_t roman_vals[13] = {
    1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
};
static const unsigned char roman_mask[13] = {
    S_M,          /* 1000 -> M */
    S_C | S_M,    /* 900  -> C M */
    S_D,          /* 500  -> D */
    S_C | S_D,    /* 400  -> C D */
    S_C,          /* 100  -> C */
    S_X | S_C,    /* 90   -> X C */
    S_L,          /* 50   -> L */
    S_X | S_L,    /* 40   -> X L */
    S_X,          /* 10   -> X */
    S_X | S_I,    /* 9    -> X I */
    S_V,          /* 5    -> V */
    S_V | S_I,    /* 4    -> V I */
    S_I           /* 1    -> I */
};

/* Compute the set of Roman symbols used in the representation of n */
static unsigned char roman_symbol_mask(seq_t n)
{
    unsigned char mask = 0;
    seq_t rem = n;
    for (idx_t i = 0; i < 13; ++i) {
        if (rem < roman_vals[i]) continue;
        seq_t cnt = rem / roman_vals[i];
        mask |= roman_mask[i];
        rem -= cnt * roman_vals[i];
    }
    return mask;
}

/* Test if n is divisible by every Roman numeral symbol it contains */
static bool good_value(seq_t n)
{
    unsigned char mask = roman_symbol_mask(n);

    if (mask & S_V  && (n % 5)   != 0) return false;
    if (mask & S_X  && (n % 10)  != 0) return false;
    if (mask & S_L  && (n % 50)  != 0) return false;
    if (mask & S_C  && (n % 100) != 0) return false;
    if (mask & S_D  && (n % 500) != 0) return false;
    if (mask & S_M  && (n % 1000)!= 0) return false;
    /* I always divides n */
    return true;
}

/* Main access function for the sequence */
seq_t a(idx_t n)
{
    /* Cache of all valid terms up to 3999 */
    static seq_t cache[4000];
    static size_t terms = 0;
    static bool initialized = false;

    if (!initialized) {
        for (idx_t num = 1; num <= 3999; ++num) {
            if (good_value((seq_t)num)) {
                cache[terms++] = (seq_t)num;
            }
        }
        initialized = true;
    }

    if (n == 0) return 0;                    /* out of bounds */
    size_t idx = (size_t)(n - 1);
    if (idx < terms) return cache[idx];
    return 0;                                 /* no further terms */
}
