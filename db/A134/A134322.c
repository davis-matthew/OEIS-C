/*
 * A134322  Positive integers with fewer non-isolated divisors than isolated
 *          divisors. A divisor, k, of n is non-isolated if (k-1) or (k+1)
 *          also divides n. A divisor, k, of n is isolated if neither (k-1)
 *          nor (k+1) divides n.
 * https://oeis.org/A134322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate integers in increasing order and keep those with
 * fewer non-isolated divisors than isolated divisors.  The function maintains
 * a static cache of found terms so that repeated calls with arbitrary
 * indices are fast.  It works for any seq_t under the constraints given. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *terms = NULL;          /* array of terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* allocated capacity of array */
static unsigned long long last_checked = 0;  /* last integer examined */

/* ------------------------------------------------------------------ */
/* Check whether a given unsigned long long m satisfies the sequence
 * criterion: the count of non‑isolated divisors is strictly less than
 * the count of isolated divisors. */
static bool is_good(unsigned long long m)
{
    size_t non_iso = 0;
    size_t iso     = 0;
    unsigned long long i;

    for (i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            /* divisor d1 = i */
            bool non = false;
            if (i > 1 && m % (i - 1) == 0) non = true;
            if (m % (i + 1) == 0) non = true;
            if (non) ++non_iso; else ++iso;

            unsigned long long d2 = m / i;
            if (d2 != i) {
                bool non2 = false;
                if (d2 > 1 && m % (d2 - 1) == 0) non2 = true;
                if (m % (d2 + 1) == 0) non2 = true;
                if (non2) ++non_iso; else ++iso;
            }
        }
    }
    return non_iso < iso;
}

/* ------------------------------------------------------------------ */
/* Public function: return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    if (n == 0) return 0;   /* safety, though undefined per spec */

    /* Extend the cache until it holds at least n terms */
    while (terms_len < (size_t)n) {
        unsigned long long m = last_checked + 1;
        last_checked = m;

        if (is_good(m)) {
            /* Grow capacity if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) { /* handle allocation failure gracefully */
                    return 0;
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
    }

    return terms[n - 1];
}
