/*
 * A059504  Numbers k with the property that reading k gives correct, although
 *          not necessarily complete, information about itself.
 * https://oeis.org/A059504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, word, nice
 * Formula:   a(n) enumerates numbers whose decimal digits can be split into
 *            pairs (c,d) where the digit d occurs exactly c times in the
 *            number. The sequence starts at 10
 * Range:     u8 n <= 10, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t a_cache[20000];      /* store computed terms (max 20000) */
static size_t a_len = 0;          /* number of stored terms */
static seq_t last_checked = (seq_t)9; /* number after last tested integer */

/* test whether a decimal representation of n satisfies the defining
   property; n is not expected to be zero. */
static bool satisfies(seq_t n)
{
    /* extract digits (least significant first) */
    seq_t tmp = n;
    char digs[256];
    size_t len = 0;
    if (tmp == (seq_t)0) {
        digs[0] = 0;
        len = 1;
    } else {
        while (tmp > (seq_t)0) {
            digs[len++] = (char)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
    }

    if ((len & 1) != 0)          /* odd length => cannot form pairs */
        return false;

    /* count occurrences of each digit 0-9 */
    unsigned int count[10] = {0};
    for (size_t i = 0; i < len; ++i)
        ++count[(unsigned char)digs[i]];

    /* verify each pair (c,d) */
    for (size_t i = len; i > 1; i -= 2) {
        unsigned int c = (unsigned char)digs[i - 1];
        unsigned int d = (unsigned char)digs[i - 2];
        if (count[d] != c)
            return false;
    }
    return true;
}

/* generate terms until at least n are known */
static void ensure_terms(idx_t n)
{
    while (a_len < (size_t)n) {
        last_checked = last_checked + (seq_t)1;
        if (satisfies(last_checked))
            a_cache[a_len++] = last_checked;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    ensure_terms(n);
    return a_cache[n - 1];
}
