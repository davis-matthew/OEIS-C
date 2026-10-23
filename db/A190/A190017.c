/*
 * A190017  Inverse permutation to A190016: lexicographical ordering of
 *          integers 1 .. 10^4.
 * https://oeis.org/A190017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = rank of n in the lexicographic ordering of the integers 1
 *            .. 10⁴
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the inverse permutation of the lexicographic
   permutation once and returns the rank for any argument n (1 ≤ n ≤ 10⁴). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration of helper. */
static void _dfs(uint64_t pref, seq_t *map, size_t *pos);

/* Build the inverse lexicographic permutation once. */
static seq_t *build_inverse(void) {
    /* Array indices : 0 .. 10 000, value at index n is a(n). */
    static seq_t map[10001];
    static bool initialized = false;
    if (!initialized) {
        size_t pos = 1; /* position starts from 1 (offset 1). */
        _dfs(0, map, &pos);
        initialized = true;
    }
    return map;
}

/* Depth‑first construction of the lexicographic order. */
static void _dfs(uint64_t pref, seq_t *map, size_t *pos) {
    if (pref > 10000)
        return;

    if (pref > 0) {
        /* Store the rank of the current number. */
        map[(int)pref] = (seq_t)*pos;
        (*pos)++;
    }

    /* Next digits: 1..9 for the leading position, 0..9 elsewhere. */
    int start = (pref == 0) ? 1 : 0;
    for (int d = start; d <= 9; d++) {
        uint64_t next = pref * 10ull + (uint64_t)d;
        if (next > 10000)
            continue;
        _dfs(next, map, pos);
    }
}

/* Public function required by the specification. */
seq_t a(idx_t n) {
    /* Guard against out of bounds input; behaviour unspecified for n > 10⁴. */
    if (n == 0 || n > 10000)
        return (seq_t)0;

    seq_t *inv = build_inverse();
    return inv[(int)n];
}
