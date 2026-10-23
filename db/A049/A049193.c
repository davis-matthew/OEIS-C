/*
 * A049193  Start with 1. Convert to base 3, describe it in base 3, convert to
 *          base 10.
 * https://oeis.org/A049193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is generated iteratively. Start with a(1)=1. For each term
 *            x, write it in base 3, run‑length encode the base‑3 digits (each
 *            run length written in base 3 followed by the run digit), and
 *            interpret the resulting decimal string as a decimal integer
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap  = 0;        /* capacity of cache array */

/* Grow cache to hold at least 'needed' terms. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute the next term from the current value. */
static seq_t next_value(seq_t cur)
{
    /* 1. Convert cur to base‑3 digits, array most‑significant first. */
    size_t dcap = 16;
    uint8_t *digits = (uint8_t *)malloc(dcap);
    size_t dlen = 0;
    seq_t tmp = cur;

    if (tmp == 0) {
        digits[dlen++] = 0;
    } else {
        while (tmp > 0) {
            uint8_t rem = (uint8_t)(tmp % 3);
            if (dlen == dcap) {
                dcap <<= 1;
                digits = (uint8_t *)realloc(digits, dcap);
            }
            digits[dlen++] = rem;
            tmp /= 3;
        }
    }

    /* Reverse to most‑significant first. */
    for (size_t i = 0; i < dlen / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[dlen - 1 - i];
        digits[dlen - 1 - i] = t;
    }

    /* 2. Run‑length encode. */
    seq_t result = 0;
    size_t i = 0;
    while (i < dlen) {
        uint8_t digit = digits[i];
        size_t j = i + 1;
        while (j < dlen && digits[j] == digit) ++j;
        size_t runlen = j - i;

        /* Encode runlen in base‑3. */
        uint8_t runbuf[48]; /* 48 digits are enough for any reasonable runlen */
        size_t runlen_digits = 0;
        size_t t = runlen;
        if (t == 0) {           /* should not happen */
            runbuf[runlen_digits++] = 0;
        } else {
            while (t > 0) {
                runbuf[runlen_digits++] = (uint8_t)(t % 3);
                t /= 3;
            }
            /* reverse to most‑significant first */
            for (size_t k = 0; k < runlen_digits / 2; ++k) {
                uint8_t tmpk = runbuf[k];
                runbuf[k] = runbuf[runlen_digits - 1 - k];
                runbuf[runlen_digits - 1 - k] = tmpk;
            }
        }

        /* Append runlen digits to result. */
        for (size_t k = 0; k < runlen_digits; ++k)
            result = result * 10 + (seq_t)runbuf[k];

        /* Append the run digit itself. */
        result = result * 10 + (seq_t)digit;

        i = j;
    }

    free(digits);
    return result;
}

/* Public function returning the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, safety */
    ensure_cache((size_t)n);
    if (cache_size == 0) {           /* initialize first term */
        cache[0] = 1;
        cache_size = 1;
    }
    if (n > cache_size) {
        for (size_t k = cache_size; k < (size_t)n; ++k) {
            seq_t prev = cache[cache_size - 1];
            seq_t next = next_value(prev);
            cache[cache_size] = next;
            ++cache_size;
        }
    }
    return cache[n - 1];
}
