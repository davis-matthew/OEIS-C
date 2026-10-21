/*
 * A213318  Numbers with exactly 11 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9449, u32 n <= 9449, u64 n <= 9449,
 *            u128 n <= 9449, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer whose decimal representation
 * has exactly 11 non‑prime substrings.  A substring is any contiguous
 * block of decimal digits.  Leading zeros are present in the
 * substring and are considered non‑prime irrespective of the numeric
 * value.  The sequence is finite (there are 32869 terms). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "../utils/isPrimeTrial.h"
#ifdef __cplusplus
}
#endif

/* Maximum number of recorded terms – the sequence ends before
 * 10^9, and there are 32869 terms.  A tiny safety margin. */
#define MAX_TERMS 33000

static seq_t cache[MAX_TERMS];
static size_t cache_len = 0;            /* number of terms already found */

/* Next integer to test; start from 0 (the test will skip earlier ones). */
static uint32_t next_candidate = 0;

/* Count the number of non‑prime substrings of a decimal integer n.
 * Leading zeros in a substring cause that substring to be counted
 * as non‑prime regardless of its numeric value. */
static inline bool has11_nonprime_substrings(uint32_t n)
{
    /* Extract digits into a buffer in natural order (most significant first). */
    uint8_t digits[10];
    int len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        uint32_t tmp = n;
        while (tmp) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* Reverse to correct order */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    int nonprime = 0;
    for (int i = 0; i < len; ++i) {
        if (digits[i] == 0) {
            /* Every substring starting at a zero digit is non‑prime. */
            nonprime += (len - i);
            if (nonprime > 11)
                return false;
            continue;
        }

        /* Build the numeric value of the substring incrementally. */
        uint32_t val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + digits[j];
            if (!isPrimeTrial((seq_t)val)) {
                ++nonprime;
                if (nonprime > 11)
                    return false;
            }
        }
    }
    return nonprime == 11;
}

/* Ensure that at least n terms have been generated and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    while (cache_len < (size_t)n) {
        /* Advance until a valid candidate is found. */
        while (!has11_nonprime_substrings(next_candidate)) {
            ++next_candidate;
            if (next_candidate == UINT32_MAX) /* safety guard */
                return;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                   /* offset is 1 – undefined for n=0 */

    if ((size_t)n > MAX_TERMS)            /* sequence is finite */
        return (seq_t)0;

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
