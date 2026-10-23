/*
 * A030285  Palindromes whose digits do not appear in previous term.
 * https://oeis.org/A030285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0 a(n) = smallest palindrome > a(n‑1) whose decimal
 *            digits do not appear in a(n‑1). The sequence is defined for n ≥
 *            1
 * Range:     u8 n <= 21, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_DIGITS 256

/* Returns the smallest palindrome >= x  (x ≥ 0).  */
static seq_t next_palindrome(seq_t x)
{
    /* Build decimal digits of x into most‑significant‑first array. */
    int rev[MAX_DIGITS];
    int revlen = 0;
    seq_t tmp = x;
    if (tmp == 0) {
        rev[revlen++] = 0;
    } else {
        while (tmp > (seq_t)0) {
            rev[revlen++] = (int)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
    }

    /* Copy to most‑significant‑first order. */
    int digits[MAX_DIGITS];
    int len = revlen;
    for (int i = 0; i < len; ++i) {
        digits[i] = rev[len - 1 - i];
    }

    /* Mirror to form a palindrome. */
    for (int i = 0; i < len / 2; ++i) {
        digits[len - 1 - i] = digits[i];
    }

    /* Build the mirrored number. */
    seq_t cand = 0;
    for (int i = 0; i < len; ++i) {
        cand = cand * (seq_t)10 + (seq_t)digits[i];
    }

    if (cand >= x) {
        return cand;
    }

    /* Increment the middle and propagate carry. */
    int carry = 1;
    int i = (len - 1) / 2;
    while (i >= 0 && carry) {
        int v = digits[i] + carry;
        digits[i] = v % 10;
        carry = v / 10;
        --i;
    }

    if (carry) {                 /* extra leading 1 needed */
        for (int j = len; j > 0; --j) {
            digits[j] = digits[j - 1];
        }
        digits[0] = 1;
        ++len;
    }

    /* Mirror again after increment. */
    for (int i = 0; i < len / 2; ++i) {
        digits[len - 1 - i] = digits[i];
    }

    /* Convert back to number. */
    seq_t res = 0;
    for (int i = 0; i < len; ++i) {
        res = res * (seq_t)10 + (seq_t)digits[i];
    }
    return res;
}

/* Checks whether x uses only digits not listed in forb[].  */
static bool digits_allowed(seq_t x, bool forb[10])
{
    if (x == (seq_t)0) {
        return !forb[0];
    }
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        if (forb[d]) {
            return false;
        }
        x /= (seq_t)10;
    }
    return true;
}

/* Sequence A030285 */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;          /* undefined for n<1, harmless return */
    }

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;     /* number of terms computed */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) {            /* allocation failure */
            abort();
        }
        cache[0] = (seq_t)0;     /* a(1) = 0 */
        size    = 1;
    }

    /* Grow cache if we need to reach n terms. */
    while (size < n) {
        seq_t prev = cache[size - 1];

        /* Mark forbidden digits from previous term. */
        bool forb[10] = { false };
        seq_t tmp = prev;
        if (tmp == (seq_t)0) {
            forb[0] = true;
        } else {
            while (tmp > (seq_t)0) {
                int d = (int)(tmp % (seq_t)10);
                forb[d] = true;
                tmp /= (seq_t)10;
            }
        }

        /* Find the smallest palindrome > prev that avoids those digits. */
        seq_t cand = next_palindrome(prev + (seq_t)1);
        while (!digits_allowed(cand, forb)) {
            cand = next_palindrome(cand + (seq_t)1);
        }

        /* Store the new term. */
        if (size >= cap) {
            size_t newcap = cap * 2;
            if (newcap < size + 2) newcap = size + 2;
            seq_t *tmpc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmpc) {
                abort();
            }
            cache = tmpc;
            cap   = newcap;
        }
        cache[size] = cand;
        ++size;
    }

    return cache[n - 1];
}
