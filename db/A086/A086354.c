/*
 * A086354  Fixed point if (nonzero-digit product)-function at initial value
 *          2^n is iterated.
 * https://oeis.org/A086354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A086354
 *
 *  a(n) = fixed point of the “non‑zero digit product” iteration
 *  applied to 2^n (offset 1).  The routine uses only dynamic
 *  arrays of decimal digits and can be compiled as pure C or
 *  C++ (seq_t may be an arbitrary‑precision integer class),
 *  avoiding any cast‑related issues by explicitly casting the
 *  void* returned from malloc/realloc.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul2.h"

/* ------------------------------------------------------------------ */
/* multiply a little‑endian decimal big integer (digits in *arr) by 2 */

/* ------------------------------------------------------------------ */
/* multiply a little‑endian decimal big integer by a small factor (1…9) */
/* ------------------------------------------------------------------ */
static void mul_small(uint8_t **arr, size_t *len, size_t *cap, uint8_t fact)
{
    if (fact == 1U) return;
    uint32_t carry = 0U;
    for (size_t i = 0; i < *len; ++i) {
        uint32_t v = (*arr)[i] * fact + carry;
        (*arr)[i] = (uint8_t)(v % 10U);
        carry      = v / 10U;
    }
    while (carry) {
        if (*len == *cap) {
            *cap = (*cap ? *cap * 2 : 1);
            *arr = (uint8_t *)realloc(*arr, *cap);
        }
        (*arr)[*len] = (uint8_t)(carry % 10U);
        carry        /= 10U;
        ++(*len);
    }
}

/* ------------------------------------------------------------------ */
/*                 public function required by the template           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The offset is 1; a(1) = 2.  For n == 0 we return 0 arbitrarily. */
    if (n == 0) return (seq_t)0;

    /* Build 2^n as a decimal number stored little‑endian. */
    size_t cap = 8, len = 1;
    uint8_t *num = (uint8_t *)malloc(cap);
    if (!num) return (seq_t)0;   /* allocation failure -> arbitrary value */
    num[0] = 1U;                 /* start with 1 */

    for (idx_t i = 0; i < n; ++i)
        mul2(&num, &len, &cap);

    /* Iterate the non‑zero digit product until a single digit is obtained. */
    while (1) {
        /* Build the product of non‑zero digits. */
        size_t pcap = 8, plen = 1;
        uint8_t *prod = (uint8_t *)malloc(pcap);
        if (!prod) { free(num); return (seq_t)0; }
        prod[0] = 1U;                        /* multiplicative identity */

        for (size_t i = 0; i < len; ++i) {
            uint8_t d = num[i];
            if (d == 0U) continue;
            mul_small(&prod, &plen, &pcap, d);
        }

        /* If the product is a single digit, we have reached the fixed point. */
        if (plen == 1U) {
            seq_t result = (seq_t)prod[0];
            free(num);
            free(prod);
            return result;
        }

        /* Continue with the new number. */
        free(num);
        num  = prod;
        len  = plen;
        cap  = pcap;
    }
}
