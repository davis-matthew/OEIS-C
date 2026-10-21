/*
 * A203566  Numbers that contain the product of any two adjacent digits as a
 *          substring, and have at least one pair of adjacent digits > 1.
 * https://oeis.org/A203566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   append_candidate, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203566 – numbers that contain the product of every two adjacent digits
 *  as a substring and have at least one pair of adjacent digits > 1.
 *  The function searches integers sequentially, keeping a cached list of
 *  found terms so that any a(n) can be returned in O(1) after the first
 *  search up to that n.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/append_candidate.h"   /* append_candidate()   */
#include "../utils/get_digits.h"         /* get_digits()         */

static seq_t *cache = NULL;
static size_t  cache_size  = 0;
static size_t  cache_cap   = 0;
static idx_t   next_candidate = 1;

/* Helper: test whether a number satisfies the definition. */
static bool valid_candidate(idx_t candidate)
{
    seq_t x = (seq_t)candidate;          /* cast to seq_t for the utils */
    uint8_t dig[128];
    size_t dlen = 0;
    get_digits(x, dig, &dlen);
    if (dlen < 2) return false;           /* need at least one adjacent pair */

    /* Build a null‑terminated string of the digits in normal order. */
    char s[128];
    for (size_t i = 0; i < dlen; ++i)
        s[i] = dig[dlen - 1 - i] + '0';
    s[dlen] = '\0';

    bool has_gt1_adj = false;

    for (size_t i = 0; i + 1 < dlen; ++i) {
        int d1 = s[i]     - '0';
        int d2 = s[i + 1] - '0';

        if (d1 > 1 && d2 > 1) has_gt1_adj = true;

        int prod = d1 * d2;
        char prodbuf[3];
        if (prod < 10) {
            prodbuf[0] = '0' + prod;
            prodbuf[1] = '\0';
        } else {
            prodbuf[0] = '0' + prod / 10;
            prodbuf[1] = '0' + prod % 10;
            prodbuf[2] = '\0';
        }

        if (strstr(s, prodbuf) == NULL)
            return false;                 /* product not found as substring */
    }

    return has_gt1_adj;
}

/* Main sequence access function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, return 0 */

    /* Generate additional terms until we have at least n. */
    while (cache_size < n) {
        if (valid_candidate(next_candidate)) {
            append_candidate(&cache, &cache_size, &cache_cap, (seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
