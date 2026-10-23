/*
 * A175547  Lexicographically earliest sequence of increasing 9-digit zeroless
 *          anagrams that share no common digit place with previous terms.
 * https://oeis.org/A175547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: lexicographically earliest increasing sequence of 9‑digit
   permutations of 1‑9 such that in each digit position all used digits
   are distinct across terms.  The first nine terms are the only ones
   possible; for n > 9 the function returns 0. */

#include <stdbool.h>

static bool built = false;
static seq_t cache[9];
static unsigned used_cols[9];   /* bits 1<<digit represent used digits per column */

/* next_permutation for an array of int of length n.
   Returns true if a next permutation exists, false if the array is the last one. */
static bool next_perm(int *p, int n)
{
    int i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (p[j] <= p[i]) j--;
    int tmp = p[i]; p[i] = p[j]; p[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = p[l]; p[l] = p[r]; p[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Build the full sequence once. */
static void build_sequence(void)
{
    if (built) return;

    int perm[9];
    for (int i = 0; i < 9; ++i) perm[i] = i + 1;
    for (int i = 0; i < 9; ++i) used_cols[i] = 0;

    /* first term */
    seq_t val = 0;
    for (int i = 0; i < 9; ++i) {
        val = val * 10 + perm[i];
        used_cols[i] |= 1u << perm[i];
    }
    cache[0] = val;

    int term = 1;
    while (term < 9) {
        /* continue from current permutation */
        while (next_perm(perm, 9)) {
            seq_t cand = 0;
            bool ok = true;
            for (int i = 0; i < 9; ++i) {
                int d = perm[i];
                if (used_cols[i] & (1u << d)) { ok = false; break; }
                cand = cand * 10 + d;
            }
            if (!ok) continue;
            if (cand > cache[term - 1]) {
                cache[term] = cand;
                for (int i = 0; i < 9; ++i) used_cols[i] |= 1u << perm[i];
                term++;
                break;
            }
        }
    }

    built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, below offset is undefined */
    if (n > 9) return 0;             /* only nine terms can exist */

    build_sequence();
    return cache[n - 1];
}
