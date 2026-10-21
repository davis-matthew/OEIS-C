/*
 * A235591  Numbers which on division by their digital root give quotient
 *          which is divisible by its digital root.
 * https://oeis.org/A235591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search algorithm generating the sequence of numbers n such that
   n / dr(n) has a quotient divisible by its digital root. */

#include <stddef.h>
#include <stdlib.h>

/* Compute the digital root (for a positive seq_t value). */
static inline uint8_t digital_root(seq_t x)
{
    return (uint8_t)(1 + ((x - 1) % 9));
}

/* Return the n‑th term of A235591 (1‑indexed).  The function lazily
   builds a cache of found terms as needed.  Any index ≥1 is supported. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next   = 1;        /* next integer to test */

    while (len < n) {
        seq_t cand = next;

        uint8_t dr = digital_root(cand);
        if (cand % dr == 0) {
            seq_t q   = cand / dr;
            uint8_t drq = digital_root(q);
            if (q % drq == 0) {
                if (len == cap) {
                    size_t newcap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap   = newcap;
                }
                cache[len++] = cand;
            }
        }
        ++next;
    }

    return cache[n - 1];
}
