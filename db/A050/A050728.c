/*
 * A050728  Decimal expansion of 7^n contains no pair of consecutive equal
 *          digits (probably finite).
 * https://oeis.org/A050728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth non‑negative integer k (starting from 0) such that
 *   the decimal expansion of 7^k has no pair of consecutive equal digits;
 *   a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=8,
 *   a(8)=9, a(9)=10, a(10)=12, a(11)=17, a(12)=27, …
 *
 * The function builds the powers of 7 incrementally on a decimal big integer
 * array, tests the result for adjacent equal digits and stores the indices
 * that satisfy the condition.  The table of valid exponents is expanded
 * lazily as needed, so repeated calls with arbitrary n values are fast.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* The build supplies the types seq_t and idx_t.  They are unsigned. */
seq_t a(idx_t n) {
    /* static state used for incremental generation */
    static bool inited = false;

    /* decimal big integer (little‑endian: digits[0] = least significant) */
    static uint8_t *digits = NULL;
    static size_t digit_len = 0, digit_cap = 0;

    /* table of exponents that satisfy the digit condition */
    static seq_t *valid = NULL;
    static idx_t v_count   = 0;        /* number stored */
    static idx_t v_capacity = 0;       /* allocated size */

    static idx_t current_exp = 0;      /* exponent represented by digits */

    /* one‑time initialisation */
    if (!inited) {
        /* 7^0 = 1 */
        digit_cap = 16;
        digits = (uint8_t *)malloc(digit_cap);
        digits[0] = 1;                 /* units digit */
        digit_len = 1;

        v_capacity = 16;
        valid = (seq_t *)malloc(v_capacity * sizeof(seq_t));
        valid[0] = 0;                  /* exponent 0 is valid */
        v_count = 1;

        inited = true;
    }

    /* generate further powers until we have at least n+1 valid exponents */
    while (v_count <= n) {
        /* multiply current number by 7 to obtain next power of 7 */
        ++current_exp;                   /* exponent of the new number */

        uint8_t carry = 0;
        for (size_t i = 0; i < digit_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 7u + carry;
            digits[i] = (uint8_t)(prod % 10u);
            carry = (uint8_t)(prod / 10u);
        }
        while (carry) {
            if (digit_len == digit_cap) {
                digit_cap <<= 1;             /* double capacity */
                digits = (uint8_t *)realloc(digits, digit_cap);
            }
            digits[digit_len++] = carry % 10u;
            carry /= 10u;
        }

        /* test for neighbouring equal decimal digits (little‑endian works) */
        bool ok = true;
        for (size_t i = 1; i < digit_len; ++i) {
            if (digits[i] == digits[i - 1]) {
                ok = false;
                break;
            }
        }

        if (ok) {          /* this exponent satisfies the condition */
            if (v_count == v_capacity) {
                v_capacity <<= 1;            /* double capacity */
                valid = (seq_t *)realloc(valid, v_capacity * sizeof(seq_t));
            }
            valid[v_count++] = (seq_t)current_exp;
        }
    }

    return valid[n];
}
