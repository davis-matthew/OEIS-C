/*
 * A053083  Number of ways of getting 5 of a kind, royal flush, other straight
 *          flush, 4 of a kind, full house, other flush, other straight, 3 of
 *          a kind, 2 pair, a pair or no pair in 5-card poker with two jokers
 *          wild.
 * https://oeis.org/A053083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   OEIS A053083 – counting 5‑card poker hands with two jokers (2
 *            joker wild cards) in the order: 5‑of‑a‑kind, royal flush, other
 *            straight flush, 4‑of‑a‑kind, full house, other flush, other
 *            straight, 3‑of‑a‑kind, 2‑pair, pair, no pair. The counting is
 *            performed by enumerating all 54 choose 5 hands an...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   can_full_house, can_one_pair, can_two_pair, match_full_house,
 *            match_one_pair, match_two_pair, popcount16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/can_full_house.h"
#include "../utils/can_one_pair.h"
#include "../utils/can_two_pair.h"
#include "../utils/match_full_house.h"
#include "../utils/match_one_pair.h"
#include "../utils/match_two_pair.h"
#include "../utils/popcount16.h"

/* 5‑card straight masks (Ace low and high are both valid) */
static const uint16_t straight_masks[10] = {
    0x001F, /* A 2 3 4 5   */
    0x003E, /* 2 3 4 5 6   */
    0x007C, /* 3 4 5 6 7   */
    0x00F8, /* 4 5 6 7 8   */
    0x01F0, /* 5 6 7 8 9   */
    0x03E0, /* 6 7 8 9 10  */
    0x07C0, /* 7 8 9 10 J  */
    0x0F80, /* 8 9 10 J Q */
    0x1F00, /* 9 10 J Q K */
    0x1E01  /* 10 J Q K A */
};
static const uint16_t royal_mask = 0x1E01; /* 10 J Q K A */

static bool straight_possible(uint16_t mask, uint16_t jokers)
{
    int uniq = popcount16(mask);
    if (5 - uniq > jokers) return false;
    for (int i = 0; i < 10; ++i)
        if ((mask & ~straight_masks[i]) == 0) return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* hand classification, returns 0..10 */
static int hand_category(idx_t hand[5])
{
    int jokers = 0;
    int ranks[5], suits[5];
    bool suit_is_joker[5] = {false};

    for (int i = 0; i < 5; ++i) {
        idx_t h = hand[i];
        if (h >= 52) {          /* jokers are 52,53 */
            suit_is_joker[i] = true;
            ++jokers;
        } else {
            suit_is_joker[i] = false;
            suits[i] = h / 13;    /* 0..3 */
            ranks[i] = h % 13;    /* 0..12 (Ace=0) */
        }
    }

    int rankcnt[13] = {0};
    for (int i = 0; i < 5; ++i)
        if (!suit_is_joker[i]) ++rankcnt[ranks[i]];

    /* 0. Five of a kind */
    if (jokers >= 1) {
        bool same = true;
        int first = -1;
        for (int i = 0; i < 5; ++i) {
            if (!suit_is_joker[i]) {
                if (first == -1) first = ranks[i];
                else if (ranks[i] != first) { same = false; break; }
            }
        }
        if (same && (5 - jokers) >= 3 && (5 - jokers) <= 4) return 0;
    }

    /* 1. Royal flush */
    for (int S = 0; S < 4; ++S) {
        bool bad = false;
        uint16_t mask = 0;
        for (int i = 0; i < 5; ++i) {
            if (!suit_is_joker[i]) {
                if (suits[i] != S) { bad = true; break; }
                mask |= 1U << ranks[i];
            }
        }
        if (bad) continue;
        if ((mask & ~royal_mask) == 0 && 5 - popcount16(mask) <= jokers)
            return 1;
    }

    /* 2. Other straight flush */
    for (int S = 0; S < 4; ++S) {
        bool bad = false;
        uint16_t mask = 0;
        for (int i = 0; i < 5; ++i) {
            if (!suit_is_joker[i]) {
                if (suits[i] != S) { bad = true; break; }
                mask |= 1U << ranks[i];
            }
        }
        if (bad) continue;
        if (straight_possible(mask, jokers))
            return 2;
    }

    /* 3. Four of a kind */
    for (int r = 0; r < 13; ++r)
        if (rankcnt[r] + jokers >= 4) return 3;

    /* 4. Full house */
    if (can_full_house(rankcnt, jokers)) return 4;

    /* 5. Other flush (not a straight flush) */
    if (rankcnt) { /* at least one non‑joker */
        int first_suit = -1;
        bool all_same = true;
        for (int i = 0; i < 5; ++i) {
            if (!suit_is_joker[i]) {
                if (first_suit == -1) first_suit = suits[i];
                else if (suits[i] != first_suit) { all_same = false; break; }
            }
        }
        if (all_same) return 5;
    }

    /* 6. Other straight (not a flush) */
    {
        uint16_t mask = 0;
        for (int i = 0; i < 5; ++i)
            if (!suit_is_joker[i]) mask |= 1U << ranks[i];
        if (straight_possible(mask, jokers)) return 6;
    }

    /* 7. Three of a kind (not full house or four of a kind) */
    for (int r = 0; r < 13; ++r)
        if (rankcnt[r] + jokers >= 3) return 7;

    /* 8. Two pair */
    if (can_two_pair(rankcnt, jokers)) return 8;

    /* 9. One pair */
    if (can_one_pair(rankcnt, jokers)) return 9;

    /* 10. No pair */
    return 10;
}

/* ------------------------------------------------------------------ */
/* cache all counts for the 11 categories */
static seq_t  counts[11];
static bool   initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        const idx_t C = 54;
        for (idx_t a = 0; a < C - 4; ++a)
            for (idx_t b = a + 1; b < C - 3; ++b)
                for (idx_t c = b + 1; c < C - 2; ++c)
                    for (idx_t d = c + 1; d < C - 1; ++d)
                        for (idx_t e = d + 1; e < C; ++e) {
                            idx_t hand[5] = {a, b, c, d, e};
                            int cat = hand_category(hand);
                            ++counts[cat];
                        }
        initialized = true;
    }
    if (n < 1 || n > 11) return 0;
    return counts[n - 1];
}
