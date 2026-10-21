/*
 * A217160  a(n) is the least value of k such that the decimal expansion of
 *          n^k contains five consecutive identical digits.
 * https://oeis.org/A217160
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_digits_uint32, has_five_consecutive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A217160 by brute‑force search. */
/* a(n) = min k such that the decimal expansion of n^k contains five consecutive equal digits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/count_digits_uint32.h"
#include "../utils/has_five_consecutive.h"

/* ------------------------------------------------------------------- */
/* Big integer in base 1e9. Only the operations required by the routine */
/* ------------------------------------------------------------------- */
#define BIGINT_BASE 1000000000U

typedef struct {
    uint32_t *limb; /* least significant limb first */
    size_t    len;  /* number of limbs */
} BigInt;

static void bigint_init(BigInt *a, uint64_t val)
{
    /* allocate one limb first, then grow if needed */
    a->limb = (uint32_t *)malloc(sizeof(uint32_t));
    a->len  = 0;
    if (a->len == 0) {
        a->limb[0] = (uint32_t)(val % BIGINT_BASE);
        val /= BIGINT_BASE;
        a->len = 1;
        if (val == 0) {
            /* keep value 0 as a single limb 0 */
            return;
        }
        /* grow for the remaining value */
        a->limb = (uint32_t *)realloc(a->limb, 2 * sizeof(uint32_t));
        a->limb[1] = (uint32_t)(val % BIGINT_BASE);
        a->len = 2;
        val /= BIGINT_BASE;
        while (val) {
            a->limb = (uint32_t *)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
            a->limb[a->len] = (uint32_t)(val % BIGINT_BASE);
            a->len++;
            val /= BIGINT_BASE;
        }
    }
}

static void bigint_free(BigInt *a)
{
    free(a->limb);
    a->len = 0;
    a->limb = NULL;
}

static void bigint_mul_small(BigInt *a, uint64_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BIGINT_BASE);
        carry = prod / BIGINT_BASE;
    }
    while (carry) {
        a->limb = (uint32_t *)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[a->len] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
        a->len++;
    }
}

/* Convert big integer to a newly allocated decimal string.
   Caller must free the returned pointer.                      */
static char *bigint_to_str(const BigInt *a)
{
    if (a->len == 0) {
        char *s = (char *)malloc(2);
        if (s) { s[0] = '0'; s[1] = '\0'; }
        return s;
    }
    size_t ms = a->len - 1;                /* most significant limb */
    size_t len = count_digits_uint32(a->limb[ms]) + (a->len - 1) * 9;
    char *s = (char *)malloc(len + 1);
    if (!s) return NULL;
    size_t pos = 0;
    pos += sprintf(s + pos, "%u", a->limb[ms]);                 /* ms limb */
    for (int i = (int)ms - 1; i >= 0; --i) {                    /* remaining limbs */
        pos += sprintf(s + pos, "%09u", a->limb[i]);
    }
    s[len] = '\0';
    return s;
}

/* ------------------------------------------------------------------- */
/* Main function required by the build system                           */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    BigInt pow;
    bigint_init(&pow, 1);          /* initialise to 1 */
    idx_t k = 0;

    for (;;) {
        ++k;
        bigint_mul_small(&pow, n); /* pow = pow * n  */

        char *str = bigint_to_str(&pow);
        if (!str) {                /* memory allocation failure */
            bigint_free(&pow);
            return 0;
        }
        bool ok = has_five_consecutive(str);
        free(str);
        if (ok) {
            bigint_free(&pow);
            return (seq_t)k;
        }
    }
}
