/*
 * A023058  Positive numbers k such that k and 2*k are anagrams of each other
 *          in base 3 (k is written here in base 3).
 * https://oeis.org/A023058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 615, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over base‑3 numbers and keep those whose digits are a permutation of the digits of 2×k in base‑3.
   Return the decimal integer whose decimal digits equal the base‑3 digits of k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    /* current k written as base‑3 digits (little‑endian) */
    static uint8_t *digits = NULL;
    static size_t dlen = 0, dcap = 0;

    /* initialise once */
    static bool init = false;
    if (!init) {
        /* start with k = 1 in base‑3 */
        dcap = 8;
        digits = (uint8_t *)malloc(dcap);
        dlen = 1;
        digits[0] = 1;

        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        init = true;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* count digits of k */
        uint32_t cnt[3] = {0, 0, 0};
        for (size_t i = 0; i < dlen; ++i) cnt[digits[i]]++;

        /* multiply k by 2 to get 2*k in base‑3 (tmp in little‑endian) */
        uint8_t tmp[256];
        size_t tlen = 0;
        uint32_t carry = 0;
        for (size_t i = 0; i < dlen; ++i) {
            uint32_t s = digits[i] * 2 + carry;
            tmp[tlen++] = s % 3;
            carry = s / 3;
        }
        while (carry) {
            tmp[tlen++] = carry % 3;
            carry /= 3;
        }

        /* count digits of 2*k */
        uint32_t cnt2[3] = {0, 0, 0};
        for (size_t i = 0; i < tlen; ++i) cnt2[tmp[i]]++;

        /* if digit multisets agree, store the term */
        if (cnt[0] == cnt2[0] && cnt[1] == cnt2[1] && cnt[2] == cnt2[2]) {
            /* convert base‑3 digits to decimal integer whose decimal digits are the base‑3 digits */
            seq_t val = 0;
            for (size_t i = dlen; i > 0; --i) {
                val = val * 10 + digits[i-1];
            }

            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = val;
        }

        /* increment k by 1 in base‑3 */
        size_t pos = 0;
        uint32_t inc = 1;
        while (inc) {
            if (pos == dlen) {                     /* need a new most significant digit */
                if (dlen == dcap) {
                    dcap *= 2;
                    digits = (uint8_t *)realloc(digits, dcap);
                }
                digits[dlen++] = 0;
            }
            uint32_t s = digits[pos] + inc;
            digits[pos] = s % 3;
            inc = s / 3;
            ++pos;
        }
    }

    return seq[n - 1];
}
