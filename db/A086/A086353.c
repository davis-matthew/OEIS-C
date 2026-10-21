/*
 * A086353  Fixed point if nonzero-digit product of n! is iterated.
 * https://oeis.org/A086353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = fixed point of the iterated product of non‑zero decimal
 *            digits of n! a(n) = A051802[n! ]. Iteration: start with n!,
 *            compute the product of its non‑zero decimal digits; repeat the
 *            process on each intermediate result until a single digit (1…9)
 *            is reached. The final single digit is returned
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* do not define other functions or include heavy headers
   to keep the implementation minimal. */
#include <stdint.h>
#include <stddef.h>

/* maximum number of decimal digits needed for 10000! is 35660;
   use a slightly bigger buffer for safety. */
#define MAX_DIG 41000

seq_t a(idx_t n)
{
    /* handle the offset: a(1) = 1 is produced automatically. */
    (void)n; /* silence unused warning in C, used below */

    /* ---- compute n! in decimal digits (least significant first) ---- */
    uint8_t fact[MAX_DIG];
    size_t fact_len = 1;
    fact[0] = 1;                /* factorial of 0 or 1 */

    for (idx_t i = 2; i <= n; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < fact_len; ++j) {
            uint32_t prod = fact[j] * (uint32_t)i + carry;
            fact[j] = prod % 10;
            carry   = prod / 10;
        }
        while (carry) {
            fact[fact_len++] = carry % 10;
            carry /= 10;
        }
    }

    /* ---- product of non‑zero digits of n! ---- */
    uint8_t prod1[MAX_DIG];
    size_t prod_len = 1;
    prod1[0] = 1;               /* multiplicative identity */

    for (size_t j = 0; j < fact_len; ++j) {
        uint8_t d = fact[j];
        if (d == 0) continue;   /* zeros are ignored */
        uint32_t carry = 0;
        for (size_t k = 0; k < prod_len; ++k) {
            uint32_t p = prod1[k] * (uint32_t)d + carry;
            prod1[k] = p % 10;
            carry    = p / 10;
        }
        while (carry) {
            prod1[prod_len++] = carry % 10;
            carry /= 10;
        }
    }

    /* ---- iterate until the product reduces to a single digit ---- */
    uint8_t prod2[MAX_DIG];
    size_t prod2_len;

    while (prod_len > 1) {
        prod2[0] = 1;
        prod2_len = 1;
        for (size_t j = 0; j < prod_len; ++j) {
            uint8_t d = prod1[j];
            if (d == 0) continue;
            uint32_t carry = 0;
            for (size_t k = 0; k < prod2_len; ++k) {
                uint32_t p = prod2[k] * (uint32_t)d + carry;
                prod2[k] = p % 10;
                carry    = p / 10;
            }
            while (carry) {
                prod2[prod2_len++] = carry % 10;
                carry /= 10;
            }
        }
        /* move prod2 into prod1 for the next iteration */
        for (size_t i = 0; i < prod2_len; ++i) prod1[i] = prod2[i];
        prod_len = prod2_len;
    }

    /* prod1 now holds the single digit fixed point */
    return (seq_t)prod1[0];
}
