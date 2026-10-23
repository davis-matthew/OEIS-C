/*
 * A014895  n is equal to the number of 3s in all numbers <= n written in base
 *          5.
 * https://oeis.org/A014895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n such that Σ_{k=1}^{n} #3(k) = n, where #3(k) counts
 *            digit 3 in base‑5 of k
 * Range:     u8 none, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   count3_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014895: n is equal to the number of 3s in all numbers ≤ n written in base 5. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count3_base5.h"

/* static cache of found terms */
static seq_t *matches = (seq_t *)0;   /* array of a(i) values      */
static size_t matches_len = 0;        /* number of terms stored    */
static size_t matches_cap = 0;        /* capacity of the array     */

/* state while scanning integers */
static uint64_t last_scanned = 0; /* largest integer that has been processed */
static seq_t cumulative = 0;      /* cumulative count of 3's up to last_scanned */

/* ensure the matches array can hold at least n elements */
static void ensure_matches_capacity(size_t n)
{
    if (n <= matches_cap) return;
    size_t newcap = matches_cap ? (matches_cap << 1) : 16;
    while (newcap < n) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(matches, newcap * sizeof(seq_t));
    if (!tmp) exit(1);   /* out of memory */
    matches = tmp;
    matches_cap = newcap;
}

/* main routine to return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, safe guard */

    /* if already computed, just return */
    if ((size_t)n <= matches_len) return matches[(size_t)n - 1];

    /* compute more terms until we have at least n of them */
    ensure_matches_capacity((size_t)n);
    uint64_t cur;
    for (cur = last_scanned + 1; matches_len < (size_t)n; ++cur) {
        int c = count3_base5(cur);
        cumulative += (seq_t)c;
        if (cumulative == (seq_t)cur) {
            matches[matches_len++] = (seq_t)cur;
        }
    }
    last_scanned = cur - 1;   /* cur is post‑incremented in the loop */
    return matches[(size_t)n - 1];
}
