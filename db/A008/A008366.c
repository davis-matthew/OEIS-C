/*
 * A008366  Smallest prime factor is >= 17.
 * https://oeis.org/A008366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_17_rough
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 17‑rough numbers (smallest prime factor ≥ 17).
 * a(1) = 1, a(2) = 17, a(3) = 19, …  (OEIS A008366)
 * The function enumerates all integers larger than 0 and tests
 * divisibility only by primes < 17 (2, 3, 5, 7, 11, 13).  All numbers
 * passing the test are 17‑rough.  The routine keeps a static cache of
 * already found terms so that calls in any order are answered
 * efficiently.  It works for arbitrary seq_t and idx_t types. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_17_rough.h"

static seq_t *rough_terms = NULL;        /* array of found 17‑rough numbers */
static size_t rough_capacity = 0;        /* allocated capacity of rough_terms */
static size_t rough_count = 0;           /* number of terms currently stored */
static uint64_t cur_num = 1;             /* next integer to test for 17‑roughness */

/* Grow the rough_terms array to at least new_cap elements. */
static void ensure_rough_capacity(size_t new_cap) {
    if (rough_capacity >= new_cap)
        return;
    size_t next_cap = rough_capacity ? rough_capacity << 1 : 128;
    if (next_cap < new_cap)
        next_cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(rough_terms, next_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* allocation failed */
    rough_terms = tmp;
    rough_capacity = next_cap;
}

/* Generate 17‑rough numbers until we have at least n of them. */
static void generate_until(idx_t n) {
    while (rough_count < (size_t)n) {
        if (is_17_rough(cur_num)) {
            ensure_rough_capacity(rough_count + 1);
            rough_terms[rough_count++] = (seq_t)cur_num;
        }
        ++cur_num;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, but harmless */
    generate_until(n);
    return rough_terms[n - 1];
}
