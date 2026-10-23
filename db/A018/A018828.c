/*
 * A018828  Numbers n such that n is a substring of its square (both n and n
 *          squared in base 4) (written in base 10).
 * https://oeis.org/A018828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 38, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th integer that is a substring of its square
   when both numbers are written in base 4.
   Transformation: take n, write it in base 4 (digits 0‑3), write n²
   in base 4, and test whether the first digit sequence appears
   consecutively in the second.  Count such n in increasing order
   (offset 1: a(1)=0). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* maximum digits needed for base‑4 representation of a 128‑bit number:
   ⌈128 / 2⌉ = 64, use 65 for safety. */
#define MAX_DIGITS 65

/* static cache of found values – grows as needed */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 0;

/* enlarge cache array if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* check if n is a substring of n² when both are in base 4 */
static bool is_base4_substring(seq_t n)
{
    /* digits of n (most significant first) */
    uint8_t dm[MAX_DIGITS];
    size_t lenm = 0;
    seq_t tmp = n;
    do {
        dm[lenm++] = (uint8_t)(tmp & 3);
        tmp >>= 2;
    } while (tmp);

    /* digits of n² (most significant first) */
    seq_t sq = n * n;
    uint8_t dsq[MAX_DIGITS];
    size_t lensq = 0;
    seq_t tmp2 = sq;
    do {
        dsq[lensq++] = (uint8_t)(tmp2 & 3);
        tmp2 >>= 2;
    } while (tmp2);

    /* reverse to big‑endian order */
    for (size_t i = 0; i < lenm / 2; ++i) {
        uint8_t t = dm[i];
        dm[i] = dm[lenm - 1 - i];
        dm[lenm - 1 - i] = t;
    }
    for (size_t i = 0; i < lensq / 2; ++i) {
        uint8_t t = dsq[i];
        dsq[i] = dsq[lensq - 1 - i];
        dsq[lensq - 1 - i] = t;
    }

    /* search for dm inside dsq */
    if (lenm > lensq)
        return false;

    for (size_t i = 0; i + lenm <= lensq; ++i) {
        bool ok = true;
        for (size_t j = 0; j < lenm; ++j) {
            if (dm[j] != dsq[i + j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* a(1) is the first term (0), so n is 1‑based */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (is_base4_substring(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
