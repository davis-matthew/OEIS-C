/*
 * A043134  Numbers k such that 0 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043134: numbers k such that the base‑7 representation of k
 * contains adjacent digits 0 and 4 (in either order) but the base‑7
 * representation of k‑1 does not.  a(1)=28.  The function builds the
 * sequence on demand and memoises results. */
#include <stdlib.h>
#include <stdbool.h>

/* return true iff x (in base‑7) has two adjacent digits 0 and 4 */
static bool has_adjacent_04(seq_t x)
{
    /* No adjacency in numbers with fewer than two digits. */
    if (x < (seq_t)7) return false;

    /* Scan digits from least‑significant upward. */
    while (x >= (seq_t)7) {
        int d1 = (int)(x % 7);   /* current digit (less significant) */
        x /= 7;                 /* remove it */
        int d2 = (int)(x % 7);   /* next digit (more significant) */
        if ((d1 == 0 && d2 == 4) || (d1 == 4 && d2 == 0))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of found terms.  Expanded automatically as needed. */
    static seq_t *cache = NULL;
    static idx_t count = 0;       /* number of terms cached */
    static idx_t capacity = 0;    /* current capacity of cache */
    static seq_t next = 1;        /* next candidate to examine */

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        seq_t k = next++;
        if (has_adjacent_04(k)) {
            seq_t km1 = k - 1;
            if (!has_adjacent_04(km1)) {
                /* Store k in cache. */
                if (count == capacity) {
                    size_t new_cap = capacity == 0 ? 8 : capacity * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* abort on out‑of‑memory */
                    cache = tmp;
                    capacity = new_cap;
                }
                cache[count++] = k;
            }
        }
    }
    return cache[n - 1];
}
