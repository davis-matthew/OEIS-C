/*
 * A063625  Smallest k such that 8^k has exactly n 9's in its decimal
 *          representation.
 * https://oeis.org/A063625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the decimal representation of 8^k *
 *            contains exactly n digits 9. The function keeps a memoised table
 *            of * already computed values and expands it as needed. Counting
 *            digits is * done on the fly with an array of decimal digits
 *            (base‑10) that stores * the current value of...
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0: 8¹ has no 9's. */
    if (n == 0)
        return (seq_t)1;

    /* Static state for memoisation and digit handling. */
    static idx_t k_curr = 0;           /* current exponent processed */
    static idx_t *k_for_n = NULL;      /* mapping n -> minimal k */
    static idx_t k_for_n_cap = 0;      /* capacity of k_for_n */
    static idx_t max_n_seen = (idx_t)-1;/* maximum n for which we have a value */
    static uint8_t *digits = NULL;     /* decimal digits of current 8^k (LSD first) */
    static size_t digits_len = 0;      /* number of used digits */
    static size_t digits_cap = 0;      /* capacity of digits array */

    /* Helper constants. */
    const idx_t SENTINEL = (idx_t)-1;

    /* Initialise lazy structures. */
    if (k_for_n == NULL) {
        k_for_n_cap = 4;
        k_for_n = (idx_t *)malloc(k_for_n_cap * sizeof(idx_t));
        if (!k_for_n) return (seq_t)-1;
        for (idx_t i = 0; i < k_for_n_cap; ++i)
            k_for_n[i] = SENTINEL;
        max_n_seen = (idx_t)-1;
    }
    if (digits == NULL) {
        digits_cap = 1 << 12;          /* 4096, will grow as needed */
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) return (seq_t)-1;
        digits[0] = 1;                 /* 8^0 = 1 to start */
        digits_len = 1;
    }

    /* Helper to ensure k_for_n has capacity up to idx. */
    while (n >= k_for_n_cap) {
        idx_t new_cap = k_for_n_cap << 1;
        if (new_cap <= n) new_cap = n + 1;
        idx_t *tmp = (idx_t *)realloc(k_for_n, new_cap * sizeof(idx_t));
        if (!tmp) return (seq_t)-1;
        k_for_n = tmp;
        for (idx_t i = k_for_n_cap; i < new_cap; ++i)
            k_for_n[i] = SENTINEL;
        k_for_n_cap = new_cap;
    }

    /* If we already know the answer, return it. */
    if (n <= max_n_seen && k_for_n[n] != SENTINEL)
        return (seq_t)k_for_n[n];

    /* Main loop: generate successive powers of 8, count 9's. */
    while (true) {
        /* Multiply current number by 8 (digits base‑10). */
        uint64_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * 8 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0) {
            if (digits_len == digits_cap) {
                size_t new_cap = digits_cap << 1;
                uint8_t *tmp = (uint8_t *)realloc(digits,
                                                new_cap * sizeof(uint8_t));
                if (!tmp) return (seq_t)-1;
                digits = tmp;
                digits_cap = new_cap;
            }
            digits[digits_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
        ++k_curr;

        /* Count the number of 9's. */
        idx_t n9 = 0;
        for (size_t i = 0; i < digits_len; ++i)
            if (digits[i] == 9)
                ++n9;

        /* Grow mapping table if necessary. */
        while (n9 >= k_for_n_cap) {
            idx_t new_cap = k_for_n_cap << 1;
            if (new_cap <= n9) new_cap = n9 + 1;
            idx_t *tmp = (idx_t *)realloc(k_for_n,
                                          new_cap * sizeof(idx_t));
            if (!tmp) return (seq_t)-1;
            k_for_n = tmp;
            for (idx_t i = k_for_n_cap; i < new_cap; ++i)
                k_for_n[i] = SENTINEL;
            k_for_n_cap = new_cap;
        }

        if (k_for_n[n9] == SENTINEL)
            k_for_n[n9] = k_curr;

        if (n9 == n)
            return (seq_t)k_curr;

        if (n9 > max_n_seen)
            max_n_seen = n9;
    }
}
