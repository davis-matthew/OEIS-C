/*
 * A050696  At least one prime factor of composite a(n) is a substring of
 *          a(n).
 * https://oeis.org/A050696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4720, u32 n <= 4720, u64 n <= 4720,
 *            u128 n <= 4720, big any n
 * Validated: standard, pass, 75 terms through n = 4608 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A050696:  a(n) = the n‑th composite integer whose decimal
    representation contains at least one decimal substring
    equal to one of its prime factors. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Array holding already computed terms.
   20000 entries are plenty for n up to about 10000.       */
static seq_t seq_cache[20000];
static size_t cache_len = 0;
static seq_t next_candidate = (seq_t)12;

/*  Return true if decimal string of v contains that of p. */
static bool contains_substring(const seq_t v, const seq_t p)
{
    static char bufV[64], bufP[32];
    size_t lenV, lenP;
    seq_to_str(v, bufV, &lenV);
    seq_to_str(p, bufP, &lenP);
    /* Ensure null termination – seq_to_str may not add it. */
    bufV[lenV] = '\0';
    bufP[lenP] = '\0';
    return strstr(bufV, bufP) != NULL;
}

/*  Test whether a composite number x has a prime factor that
    appears as a substring of x's decimal representation. */
static bool has_property(seq_t x)
{
    /* 0, 1, 2, 3 are not composite. */
    if (x <= (seq_t)3)
        return false;

    seq_t orig = x;
    seq_t temp = orig;
    seq_t limit = floor_sqrt(temp);
    bool found_factor = false;

    /* trial division up to sqrt(x) */
    for (uint64_t i = 2; ; ++i)
    {
        seq_t i_seq = (seq_t)i;
        if (i_seq > limit)
            break;

        if (temp % i_seq == 0)
        {
            found_factor = true;
            if (contains_substring(orig, i_seq))
                return true;

            /* skip all copies of this prime factor */
            while (temp % i_seq == 0)
                temp /= i_seq;
        }
    }

    /* If no factor was found, x is prime. */
    if (!found_factor)
        return false;

    /* Remaining factor (if > 1) is also prime. */
    if (temp > (seq_t)1)
        return contains_substring(orig, temp);

    return false;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n)
    {
        if (has_property(next_candidate))
            seq_cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
