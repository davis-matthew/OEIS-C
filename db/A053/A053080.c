/*
 * A053080  Number of ways of getting 5 of a kind, royal flush, other straight
 *          flush, 4 of a kind, full house, other flush, other straight, 3 of
 *          a kind, 2 pair, a pair or no pair in 5-card poker when joker is
 *          wild.
 * https://oeis.org/A053080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the number of 5‑card hands of the n‑th hand type (1:
 *            5‑of‑a‑kind, 2: royal flush, 3: other straight flush, 4: four of
 *            a kind, 5: full house, 6: other flush, 7: other straight, 8:
 *            three of a kind, 9: two pair, 10: one pair, 11: no pair) in a
 *            53‑card deck containing one joker acting as a wi...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int seq_masks[10] = {
    0x1F,   /* 0-4      */
    0x3E,   /* 1-5      */
    0x7C,   /* 2-6      */
    0xF8,   /* 3-7      */
    0x1F0,  /* 4-8      */
    0x3E0,  /* 5-9      */
    0x7C0,  /* 6-10     */
    0xF80,  /* 7-11     */
    0x1F00, /* 8-12     */
    0x1000 | 0x0F  /* wheel: 12,0,1,2,3 */
};
static const int ROYAL_MASK = seq_masks[8];

/* classify a 5‑card hand with no joker.  Returns category 1…11. */
static inline int classify_hand(const int hand[5])
{
    int freq[13] = {0};
    int suits[5];
    int rank_mask = 0;

    for (int i = 0; i < 5; ++i) {
        int card = hand[i];
        int rank = card % 13;
        int suit = card / 13;
        suits[i] = suit;
        ++freq[rank];
        rank_mask |= (1 << rank);
    }

    /* 5‑of‑a‑kind */
    for (int r = 0; r < 13; ++r)
        if (freq[r] == 5) return 1;

    /* flush check */
    bool flush = true;
    for (int i = 1; i < 5; ++i)
        if (suits[i] != suits[0]) { flush = false; break; }

    /* straight check */
    bool straight = false;
    int distinct = 0;
    for (int r = 0; r < 13; ++r)
        if (freq[r]) ++distinct;
    if (distinct == 5) {
        for (int m = 0; m < 10; ++m)
            if (rank_mask == seq_masks[m]) { straight = true; break; }
    }

    /* royal flush / straight flush */
    if (flush && straight) {
        if (rank_mask == ROYAL_MASK) return 2;   /* royal flush */
        else return 3;                           /* other straight flush */
    }

    /* four of a kind */
    for (int r = 0; r < 13; ++r)
        if (freq[r] == 4) return 4;

    /* full house */
    int num3 = 0, num2 = 0;
    for (int r = 0; r < 13; ++r) {
        if (freq[r] == 3) ++num3;
        if (freq[r] == 2) ++num2;
    }
    if (num3 >= 1 && num2 >= 1) return 5;

    /* other flush */
    if (flush) return 6;

    /* other straight */
    if (straight) return 7;

    /* three of a kind */
    if (num3 >= 1) return 8;

    /* two pair */
    if (num2 >= 2) return 9;

    /* one pair */
    if (num2 == 1) return 10;

    return 11;  /* no pair */
}

/* compute all counts once */
static void compute_counts(seq_t *counts)
{
    counts[0] = counts[1] = counts[2] = counts[3] = counts[4] =
    counts[5] = counts[6] = counts[7] = counts[8] = counts[9] =
    counts[10] = 0;

    for (int i = 0; i < 49; ++i)          /* 53-4 */
    for (int j = i + 1; j < 50; ++j)      /* 53-3 */
    for (int k = j + 1; k < 51; ++k)      /* 53-2 */
    for (int l = k + 1; l < 52; ++l)      /* 53-1 */
    for (int m = l + 1; m < 53; ++m) {
        int hand[5] = { i, j, k, l, m };
        bool has_joker = false;
        int joker_idx = -1;
        for (int idx = 0; idx < 5; ++idx) {
            if (hand[idx] == 52) { has_joker = true; joker_idx = idx; }
        }
        if (!has_joker) {
            int cat = classify_hand(hand);
            ++counts[cat - 1];
        } else {
            int best_cat = 12;          /* worse than worst category */
            int tmp_hand[5];
            for (int r = 0; r < 13; ++r)
            for (int s = 0; s < 4; ++s) {
                int joker_card = r + 13 * s;
                for (int t = 0; t < 5; ++t) tmp_hand[t] = hand[t];
                tmp_hand[joker_idx] = joker_card;
                int cat = classify_hand(tmp_hand);
                if (cat < best_cat) {
                    best_cat = cat;
                    if (best_cat == 1) goto done; /* can't do better */
                }
            }
            done:
            ++counts[best_cat - 1];
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t counts[11];
    if (!initialized) {
        compute_counts(counts);
        initialized = true;
    }
    if (n >= 1 && n <= 11) return counts[n - 1];
    return 0;   /* undefined terms return 0 */
}
