/*
 * A065715  Number of 4's in decimal expansion of 2^n.
 * https://oeis.org/A065715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '4's in the decimal expansion of 2^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: use std::vector for automatic construction of seq_t objects. */
    static std::vector<seq_t> counts;
    static std::vector<uint8_t> digits(1, 1);  /* least‑significant digit first */
    static size_t max_computed = 0;

    if (n >= counts.size())
        counts.resize(n + 1);

    while (max_computed < n) {
        uint32_t carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            uint32_t val = (uint32_t)digits[i] * 2 + carry;
            digits[i] = val % 10;
            carry = val / 10;
        }
        while (carry) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
        seq_t cnt = 0;
        for (size_t i = 0; i < digits.size(); ++i)
            if (digits[i] == 4)
                ++cnt;
        ++max_computed;
        counts[max_computed] = cnt;
    }

    return counts[n];
#else
    /* C build: use malloc/realloc; seq_t is an unsigned integer type. */
    static seq_t *counts = NULL;
    static size_t count_cap = 0;
    static uint8_t digits[4000];
    static size_t digit_len = 0;
    static size_t max_computed = 0;

    if (!counts) {
        digits[0] = 1;          /* 2^0 = 1 */
        digit_len = 1;
        counts = (seq_t *)malloc(sizeof(seq_t));
        if (!counts)
            return 0;
        counts[0] = 0;
        count_cap = 1;
    }

    if (n >= count_cap) {
        size_t new_cap = count_cap ? count_cap << 1 : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(counts, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;
        counts = tmp;
        count_cap = new_cap;
    }

    while (max_computed < n) {
        uint16_t carry = 0;
        for (size_t i = 0; i < digit_len; ++i) {
            uint16_t val = (uint16_t)digits[i] * 2 + carry;
            digits[i] = val % 10;
            carry = val / 10;
        }
        while (carry) {
            digits[digit_len++] = carry % 10;
            carry /= 10;
        }
        seq_t cnt = 0;
        for (size_t i = 0; i < digit_len; ++i)
            if (digits[i] == 4)
                ++cnt;
        ++max_computed;
        counts[max_computed] = cnt;
    }

    return counts[n];
#endif
}
