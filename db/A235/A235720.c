/*
 * A235720  Squares which have one or more occurrences of exactly five
 *          different digits.
 * https://oeis.org/A235720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A054033(n))^2, where A054033(n) is the n‑th integer
 *            whose square has exactly five distinct decimal digits
 * Range:     u8 none, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool has_five_distinct_digits(seq_t x) {
    bool seen[10] = {false};
    int distinct = 0;
    do {
        int d = (int)(x % 10);
        if (!seen[d]) {
            seen[d] = true;
            if (++distinct > 5)
                return false;
        }
        x /= 10;
    } while (x);
    return distinct == 5;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t next_root = 1;    /* next integer to test as a potential root */

    while (cache_cnt < n) {
        /* Allocate or grow cache */
        if (cache_cnt == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – abort */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t root = (seq_t)next_root;
        seq_t square = root * root;
        if (has_five_distinct_digits(square)) {
            cache[cache_cnt++] = square;
        }
        ++next_root;
    }

    return cache[n - 1];
}
