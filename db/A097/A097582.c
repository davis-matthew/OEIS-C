/*
 * A097582  Base 7 representation of the concatenation of the first n numbers
 *          with the most significant digits first.
 * https://oeis.org/A097582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A007093(A007908(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A097582 sequence: Base 7 representation of the concatenation
   of the first n numbers with the most significant digits first. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* big integer stored in base 1 000 000 000 (least significant limb first) */
    const uint32_t BASE = 1000000000U;
    uint32_t *B = NULL;
    size_t Blen = 0;   /* number of used limbs */
    size_t Bcap = 0;   /* allocated capacity in limbs */

    /* powers of ten for up to 5 decimal digits (n ≤ 10000) */
    uint32_t pow10[6] = {1,10,100,1000,10000,100000};

    /* build the decimal concatenation as big integer */
    for (idx_t i = 1; i <= n; ++i) {
        /* number of decimal digits of i */
        uint32_t di = 0;
        uint32_t tmp = (uint32_t)i;
        while (tmp) { ++di; tmp /= 10; }
        uint32_t mult = pow10[di];

        /* multiply B by mult (10^di) */
        if (Blen > 0) {
            if (Blen == Bcap) {
                if (Bcap == 0) Bcap = 8;
                else Bcap <<= 1;
                B = (uint32_t*)realloc(B, Bcap * sizeof(uint32_t));
            }
            uint64_t carry = 0;
            for (size_t j = 0; j < Blen; ++j) {
                uint64_t prod = (uint64_t)B[j] * mult + carry;
                B[j] = (uint32_t)(prod % BASE);
                carry = prod / BASE;
            }
            while (carry) {
                if (Blen == Bcap) {
                    if (Bcap == 0) Bcap = 8;
                    else Bcap <<= 1;
                    B = (uint32_t*)realloc(B, Bcap * sizeof(uint32_t));
                }
                B[Blen++] = (uint32_t)(carry % BASE);
                carry /= BASE;
            }
        }

        /* add i to B */
        uint64_t carry = (uint64_t)i;
        size_t j = 0;
        while (carry && j < Blen) {
            uint64_t sum = (uint64_t)B[j] + carry;
            B[j] = (uint32_t)(sum % BASE);
            carry = sum / BASE;
            ++j;
        }
        while (carry) {
            if (Blen == Bcap) {
                if (Bcap == 0) Bcap = 8;
                else Bcap <<= 1;
                B = (uint32_t*)realloc(B, Bcap * sizeof(uint32_t));
            }
            B[Blen++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* convert the big integer B to base‑7 digits */
    if (Blen == 0) {
        free(B);
        return (seq_t)0;
    }

    uint8_t *base7 = NULL;
    size_t base7_len = 0;
    size_t base7_cap = 64;
    base7 = (uint8_t*)malloc(base7_cap * sizeof(uint8_t));

    while (Blen > 0) {
        /* divide B by 7; remainder becomes next least significant base‑7 digit */
        uint64_t carry = 0;
        for (size_t j = Blen; j-- > 0; ) {
            uint64_t cur = carry * BASE + B[j];
            B[j] = (uint32_t)(cur / 7);
            carry = cur % 7;
        }
        if (base7_len == base7_cap) {
            base7_cap <<= 1;
            base7 = (uint8_t*)realloc(base7, base7_cap * sizeof(uint8_t));
        }
        base7[base7_len++] = (uint8_t)carry;
        while (Blen > 0 && B[Blen-1] == 0) --Blen;
    }

    /* build the result as a decimal number whose digits are the base‑7 digits */
    seq_t result = 0;
    for (size_t i = base7_len; i-- > 0; ) {
        result = result * 10 + base7[i];
    }

    free(B);
    free(base7);

    return result;
}
