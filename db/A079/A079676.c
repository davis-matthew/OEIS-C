/*
 * A079676  LookAndSay(n) is palindromic.
 * https://oeis.org/A079676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A079676: numbers whose look‑and‑say expansion is a palindrome
   by searching consecutive integers and testing the palindrome property. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Encode an unsigned integer value into its decimal digit sequence,
   stored most significant first in *out.  *len receives the number of
   digits.  */
static void encode_decimal_size_t(size_t value, uint8_t *out, size_t *len) {
    size_t i = 0;
    do {
        out[i++] = value % 10;
        value /= 10;
    } while (value > 0);
    /* reverse to most significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        uint8_t c = out[j];
        out[j] = out[i - 1 - j];
        out[i - 1 - j] = c;
    }
    *len = i;
}

/* Return true iff the look‑and‑say transformation of x yields a palindrome. */
static bool lss_is_pal(seq_t x) {
    /* Extract decimal digits of x into revDigits (least significant first). */
    size_t capDigits = 16;
    size_t lenDigits = 0;
    uint8_t *revDigits = (uint8_t *)malloc(capDigits);
    if (!revDigits) return false;
    seq_t tmp = x;
    if (tmp == 0) {
        revDigits[0] = 0;
        lenDigits = 1;
    } else {
        while (tmp != 0) {
            if (lenDigits == capDigits) {
                capDigits <<= 1;
                revDigits = (uint8_t *)realloc(revDigits, capDigits);
                if (!revDigits) return false;
            }
            revDigits[lenDigits++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Build the look‑and‑say output in out buffer. */
    size_t capOut = 16;
    size_t lenOut = 0;
    uint8_t *out = (uint8_t *)malloc(capOut);
    if (!out) { free(revDigits); return false; }

    size_t pos = 0;
    while (pos < lenDigits) {
        uint8_t digit = revDigits[lenDigits - 1 - pos];
        size_t run = 1;
        while (pos + run < lenDigits &&
               revDigits[lenDigits - 1 - (pos + run)] == digit) {
            ++run;
        }

        /* Encode run length into decimal digits. */
        size_t rl = run;
        size_t capRL = 8;
        size_t lenRL = 0;
        uint8_t *rlDigits = (uint8_t *)malloc(capRL);
        if (!rlDigits) { free(out); free(revDigits); return false; }
        if (rl == 0) {
            rlDigits[0] = 0;
            lenRL = 1;
        } else {
            while (rl > 0) {
                if (lenRL == capRL) {
                    capRL <<= 1;
                    rlDigits = (uint8_t *)realloc(rlDigits, capRL);
                    if (!rlDigits) { free(out); free(revDigits); return false; }
                }
                rlDigits[lenRL++] = rl % 10;
                rl /= 10;
            }
        }
        /* Append run length digits in most significant first order. */
        for (size_t k = 0; k < lenRL; ++k) {
            if (lenOut == capOut) {
                capOut <<= 1;
                out = (uint8_t *)realloc(out, capOut);
                if (!out) { free(rlDigits); free(revDigits); return false; }
            }
            out[lenOut++] = rlDigits[lenRL - 1 - k];
        }
        free(rlDigits);

        /* Append the digit itself. */
        if (lenOut == capOut) {
            capOut <<= 1;
            out = (uint8_t *)realloc(out, capOut);
            if (!out) { free(revDigits); return false; }
        }
        out[lenOut++] = digit;

        pos += run;
    }

    /* Check palindrome. */
    bool isPal = true;
    for (size_t i = 0, j = lenOut; i < j; ++i, --j) {
        if (out[i] != out[j - 1]) {
            isPal = false;
            break;
        }
    }

    free(out);
    free(revDigits);
    return isPal;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* by convention */

#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic construction. */
    static std::vector<seq_t> cacheVec;
    static seq_t next_candidate = 1;

    while (cacheVec.size() < (size_t)n) {
        seq_t cand = next_candidate;
        if (lss_is_pal(cand)) {
            cacheVec.push_back(cand);
        }
        ++next_candidate;
    }
    return cacheVec[n - 1];
#else
    /* C implementation using manual malloc/realloc. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;

    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        if (lss_is_pal(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* Out of memory; propagate zero. */ return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
#endif
}
