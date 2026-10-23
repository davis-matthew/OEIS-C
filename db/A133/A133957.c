/*
 * A133957  Form the list of home primes A037274(c) for c composite, and sort
 *          into increasing order.
 * https://oeis.org/A133957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   isPrimeTrial, is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A133957 – sorted list of all primes that can be written
   as a concatenation of at least two primes in nondecreasing order.
   The code generates primes in increasing order, tests each prime
   for such a concatenation, and stores the qualifying primes in
   a static table (memoised).  The nth term is returned from this
   table.  No hard‑coded terms are used. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrimeTrial.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_to_str.h"

/* --------------------------------------------------------------------- */
/*  Test whether a prime number p can be split into at least two primes
    that appear in nondecreasing numeric order.  The function receives
    the decimal representation of p as a character array *s of length
    *len.  It returns true if such a split exists.                     */
/* --------------------------------------------------------------------- */
static bool split_rec(const char *s, size_t pos, size_t len,
                      seq_t prev, int parts)
{
    if (pos == len) return parts >= 2;                /* reached end */
    if (s[pos] == '0') return false;                  /* leading zero not allowed */

    seq_t val = 0;
    for (size_t j = pos; j < len; ++j) {
        val = val * 10 + (seq_t)(s[j] - '0');
        /* enforce nondecreasing order */
        if ((parts == 0 || val >= prev) && is_prime_big(val)) {
            if (split_rec(s, j + 1, len, val, parts + 1))
                return true;
        }
    }
    return false;
}

static bool can_split_nondec(const char *s, size_t len)
{
    return split_rec(s, 0, len, 0, 0);
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *home = NULL;
    static size_t home_count = 0;
    static size_t home_cap   = 0;
    static idx_t  candidate  = 2;          /* numbers to test, start at 2 */

    /* Allocate the home array lazily. */
    if (!home) {
        home_cap = 8;
        home = (seq_t *)malloc(home_cap * sizeof(seq_t));
        if (!home) return 0;               /* allocation failure */
    }

    /* Find new qualifying primes until we have at least n of them. */
    while (home_count < n) {
        seq_t cand = (seq_t)candidate;
        if (isPrimeTrial(cand)) {          /* test only primes */
            char buf[64];
            size_t len;
            seq_to_str(cand, buf, &len);

            if (can_split_nondec(buf, len)) {
                /* Store prime if it satisfies the split condition. */
                if (home_count == home_cap) {
                    home_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(home, home_cap * sizeof(seq_t));
                    if (!tmp) { free(home); return 0; }
                    home = tmp;
                }
                home[home_count++] = cand;
            }
        }
        ++candidate;
    }

    return home[n - 1];
}
