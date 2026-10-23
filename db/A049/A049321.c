/*
 * A049321  Primitive Chacon sequence: fixed under 0->0012, 1->12, 2->012.
 * https://oeis.org/A049321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) is the n-th term of the fixed point of the morphism 0 →
 *            0012, 1 → 12, 2 → 012 (the Primitive Chacon word)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function expected by the test harness. */
#include <stdint.h>
#include <stddef.h>

/* helper struct for storing lengths of σ^k(s) for all symbols s */
typedef struct {
    unsigned __int128 l0, l1, l2;
} lens_t;

/* child symbols for each letter according to the morphism */
static const int child_cnt[3] = {4, 2, 3};
static const int child[3][4] = {
    {0, 0, 1, 2},   /* 0 → 0012 */
    {1, 2},         /* 1 → 12   */
    {0, 1, 2}       /* 2 → 012  */
};

/* compute a(n) */
seq_t a(idx_t n) {
    /* we need to build the length table up to the smallest depth
       where length of σ^depth(0) > n.  */
    /* use a static array big enough for any n that fits into idx_t */
    static lens_t lens[70];
    int depth = 0;

    lens[0].l0 = 1;
    lens[0].l1 = 1;
    lens[0].l2 = 1;

    /* limit to prevent overflow of 128‑bit arithmetic */
    const unsigned __int128 LIMIT = (unsigned __int128)UINT64_MAX + 1ULL;

    while (lens[depth].l0 <= (unsigned __int128)n) {
        /* grow to next depth */
        depth++;
        /* compute next lengths from the previous depth */
        lens[depth].l0 = 2 * lens[depth-1].l0 + lens[depth-1].l1 + lens[depth-1].l2;
        lens[depth].l1 = lens[depth-1].l1 + lens[depth-1].l2;
        lens[depth].l2 = lens[depth-1].l0 + lens[depth-1].l1 + lens[depth-1].l2;

        /* clamp if overflow */
        if (lens[depth].l0 > LIMIT) lens[depth].l0 = LIMIT;
        if (lens[depth].l1 > LIMIT) lens[depth].l1 = LIMIT;
        if (lens[depth].l2 > LIMIT) lens[depth].l2 = LIMIT;
    }

    /* now walk back from depth to depth 0 to find the symbol at
       position n of the infinite word.  */
    int symbol = 0;                 /* start at the root symbol 0 */
    int d = depth;
    uint64_t idx = n;               /* remaining offset within current block */

    while (d > 0) {
        /* iterate over child symbols of current symbol */
        int i = 0;
        for (; i < child_cnt[symbol]; ++i) {
            int child_sym = child[symbol][i];
            unsigned __int128 child_len;
            switch (child_sym) {
                case 0: child_len = lens[d-1].l0; break;
                case 1: child_len = lens[d-1].l1; break;
                default: child_len = lens[d-1].l2; break;
            }

            if ((unsigned __int128)idx < child_len) {
                /* the wanted index lies in this child */
                symbol = child_sym;
                d--;               /* descend one level */
                break;
            } else {
                idx -= (uint64_t)child_len;   /* skip this child block */
            }
        }
    }

    /* at depth 0 each symbol is a single letter */
    return (seq_t)symbol;
}
