/*
 * A067109  Number of occurrences of the string n in n! (A000142).
 * https://oeis.org/A067109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of (possibly overlapping) occurrences of the
 *            decimal * string representation of n inside n! (n factorial).
 *            The function * recomputes n! each time it is called by
 *            multiplying successive integers * using a base‑1e9
 *            arbitrary‑precision integer. The result is obtained * by
 *            converting th...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BASE 1000000000u   /* 1e9, fits in a 32‑bit unsigned */

typedef struct {
    uint32_t *limbs;   /* little‑endian limbs, each < BASE */
    size_t     len;    /* number of used limbs */
    size_t     cap;    /* allocated limbs */
} BigInt;

static void bint_init(BigInt *b) {
    b->cap = 8;
    b->len = 1;
    b->limbs = (uint32_t*)malloc(b->cap * sizeof(uint32_t));
    b->limbs[0] = 1;   /* initial value 1 (=0! = 1) */
}

static void bint_mul_small(BigInt *b, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limbs[i] * m + carry;
        b->limbs[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        if (b->len == b->cap) {
            b->cap <<= 1;
            b->limbs = (uint32_t*)realloc(b->limbs,
                                         b->cap * sizeof(uint32_t));
        }
        b->limbs[b->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void bint_free(BigInt *b) {
    free(b->limbs);
}

/* Convert big integer to decimal string.  Caller must provide buffer
   with enough space (at least ~ d(n)+1 characters). */
static void bint_to_str(const BigInt *b, char *buf, size_t *len) {
    size_t idx = 0;
    /* most significant limb without leading zeros */
    idx += sprintf(&buf[idx], "%u", b->limbs[b->len - 1]);
    /* remaining limbs with leading zeros to width 9 */
    for (long long i = (long long)b->len - 2; i >= 0; --i)
        idx += sprintf(&buf[idx], "%09u", b->limbs[i]);
    buf[idx] = '\0';
    *len = idx;
}

seq_t a(idx_t n) {
    /* Handle small n quickly */
    if (n == 0) return 0;

    /* Compute n! */
    BigInt fac;
    bint_init(&fac);
    for (idx_t i = 2; i <= n; ++i)
        bint_mul_small(&fac, (uint32_t)i);

    /* Convert factorial to decimal string */
    char raw[47000];          /* enough for 10000! (~35700 digits)   */
    size_t raw_len = 0;
    bint_to_str(&fac, raw, &raw_len);

    /* Prepare pattern string for n */
    char pattern[32];
    sprintf(pattern, "%llu", (unsigned long long)n);
    size_t pat_len = strlen(pattern);

    /* Count overlapping occurrences of pattern in raw */
    seq_t count = 0;
    for (size_t p = 0; p + pat_len <= raw_len; ++p) {
        if (strncmp(&raw[p], pattern, pat_len) == 0)
            ++count;
    }

    bint_free(&fac);
    return count;
}
