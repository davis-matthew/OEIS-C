/*
 * A126810  Ordinal relative letter frequency in the English language of each
 *          letter of the alphabet.
 * https://oeis.org/A126810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A126810
   a(n) is the rank (1 = most frequent) of the nth letter of the English alphabet
   when letters are ordered by their frequency in English. */

#include <stdbool.h>

static const double freq[26] = {
    /* a */ 24.0,
    /* b */ 7.0,
    /* c */ 15.0,
    /* d */ 17.0,
    /* e */ 26.0,
    /* f */ 11.0,
    /* g */ 10.0,
    /* h */ 19.0,
    /* i */ 22.0,
    /* j */ 4.0,
    /* k */ 5.0,
    /* l */ 16.0,
    /* m */ 13.0,
    /* n */ 21.0,
    /* o */ 23.0,
    /* p */ 8.0,
    /* q */ 2.0,
    /* r */ 18.0,
    /* s */ 20.0,
    /* t */ 25.0,
    /* u */ 14.0,
    /* v */ 6.0,
    /* w */ 12.0,
    /* x */ 3.0,
    /* y */ 9.0,
    /* z */ 1.0
};

seq_t a(idx_t n) {
    static seq_t rank[26];
    static bool init = false;
    if (!init) {
        int idx[26];
        for (int i = 0; i < 26; ++i) idx[i] = i;

        /* sort indices by descending frequency (simple selection sort) */
        for (int i = 0; i < 25; ++i) {
            int max = i;
            for (int j = i + 1; j < 26; ++j)
                if (freq[idx[j]] > freq[idx[max]]) max = j;
            if (max != i) {
                int tmp = idx[i];
                idx[i] = idx[max];
                idx[max] = tmp;
            }
        }

        /* assign ranks based on sorted order */
        for (int i = 0; i < 26; ++i)
            rank[idx[i]] = (seq_t)(i + 1);

        init = true;
    }
    return rank[(n - 1) % 26];
}
