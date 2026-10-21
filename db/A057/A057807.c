/*
 * A057807  Number of ways of getting (at least) 5 of a kind, a straight
 *          flush, 4 of a kind, flush, full house, straight, 3 of a kind, 2
 *          pair, a pair in wild-card poker with 1 joker.
 * https://oeis.org/A057807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the nine exclusive poker hand counts for a 53‑card deck
 * (52 standard cards plus one joker).  The counts are:
 *   0: five‑of‑a‑kind
 *   1: straight flush
 *   2: four‑of‑a‑kind
 *   3: flush (not straight flush)
 *   4: full house
 *   5: straight (not flush)
 *   6: three‑of‑a‑kind
 *   7: two pair
 *   8: one pair
 * The function enumerates all 53C5 hands, classifies each hand,
 * updates the corresponding counter, and returns the requested
 * count.  For n ≥ 9 the function returns 0.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TOTAL_CARDS 53   /* 52 regular cards + 1 joker (index 52) */
#define RANKS 13
#define SUITS 4

static seq_t table[9] = {0};
static bool built = false;

static void build_table(void)
{
    for (int i0 = 0; i0 < TOTAL_CARDS - 4; ++i0)
    for (int i1 = i0 + 1; i1 < TOTAL_CARDS - 3; ++i1)
    for (int i2 = i1 + 1; i2 < TOTAL_CARDS - 2; ++i2)
    for (int i3 = i2 + 1; i3 < TOTAL_CARDS - 1; ++i3)
    for (int i4 = i3 + 1; i4 < TOTAL_CARDS; ++i4) {
        /* initialise counters */
        int rank_counts[RANKS] = {0};
        int suit_counts[SUITS] = {0};
        int suit_bitmask[SUITS] = {0};
        bool joker = false;

        int hand[5] = {i0, i1, i2, i3, i4};
        for (int h = 0; h < 5; ++h) {
            int c = hand[h];
            if (c == TOTAL_CARDS - 1) { /* joker */
                joker = true;
            } else {
                int r = c % RANKS;
                int s = c / RANKS;
                rank_counts[r]++;
                suit_counts[s]++;
                suit_bitmask[s] |= (1 << r);
            }
        }

        int jokerCnt = joker ? 1 : 0;

        /* 0 – five‑of‑a‑kind */
        bool cat = false;
        for (int r = 0; r < RANKS; ++r)
            if (rank_counts[r] + jokerCnt >= 5) {
                cat = true;
                break;
            }
        if (cat) {
            table[0] += (seq_t)1;
            continue;
        }

        /* 1 – straight flush */
        for (int s = 0; s < SUITS; ++s) {
            if (suit_counts[s] + jokerCnt < 5)
                continue;
            int bits = suit_bitmask[s];
            /* straight ranges */
            for (int start = 0; start <= RANKS - 5; ++start) {
                int mask = ((1 << 5) - 1) << start;
                int present = bits & mask;
                int missing = 5 - __builtin_popcount((unsigned)present);
                if (missing <= jokerCnt) {
                    cat = true;
                    break;
                }
            }
            if (cat)
                break;
            /* wheel: A2345 */
            int wheel = (1 << 12) | ((1 << 4) - 1);
            int present = bits & wheel;
            int missing = 5 - __builtin_popcount((unsigned)present);
            if (missing <= jokerCnt) {
                cat = true;
                break;
            }
        }
        if (cat) {
            table[1] += (seq_t)1;
            continue;
        }

        /* 2 – four‑of‑a‑kind */
        for (int r = 0; r < RANKS; ++r)
            if (rank_counts[r] + jokerCnt >= 4) {
                cat = true;
                break;
            }
        if (cat) {
            table[2] += (seq_t)1;
            continue;
        }

        /* 3 – flush (not straight flush) */
        for (int s = 0; s < SUITS; ++s)
            if (suit_counts[s] + jokerCnt >= 5) {
                cat = true;
                break;
            }
        if (cat) {
            table[3] += (seq_t)1;
            continue;
        }

        /* 4 – full house */
        if (!jokerCnt) {
            int trips = 0, pairs = 0;
            for (int r = 0; r < RANKS; ++r) {
                if (rank_counts[r] == 3)
                    trips++;
                else if (rank_counts[r] == 2)
                    pairs++;
            }
            if (trips == 1 && pairs == 1)
                cat = true;
        } else {
            for (int r0 = 0; r0 < RANKS && !cat; ++r0) {
                rank_counts[r0]++;   /* assign joker */
                int trips = 0, pairs = 0;
                for (int r = 0; r < RANKS; ++r) {
                    if (rank_counts[r] == 3)
                        trips++;
                    else if (rank_counts[r] == 2)
                        pairs++;
                }
                if (trips == 1 && pairs == 1)
                    cat = true;
                rank_counts[r0]--;   /* revert */
            }
        }
        if (cat) {
            table[4] += (seq_t)1;
            continue;
        }

        /* 5 – straight (non‑flush) */
        bool dup = false;
        for (int r = 0; r < RANKS; ++r)
            if (rank_counts[r] > 1) {
                dup = true;
                break;
            }
        if (!dup) {
            int bits = 0;
            for (int r = 0; r < RANKS; ++r)
                if (rank_counts[r])
                    bits |= (1 << r);
            if (!jokerCnt) {
                for (int start = 0; start <= RANKS - 5; ++start) {
                    int mask = ((1 << 5) - 1) << start;
                    int present = bits & mask;
                    int missing = 5 - __builtin_popcount((unsigned)present);
                    if (missing == 0) {
                        cat = true;
                        break;
                    }
                }
                if (!cat) {
                    int wheel = (1 << 12) | ((1 << 4) - 1);
                    int present = bits & wheel;
                    int missing = 5 - __builtin_popcount((unsigned)present);
                    if (missing == 0)
                        cat = true;
                }
            } else {
                for (int start = 0; start <= RANKS - 5; ++start) {
                    int mask = ((1 << 5) - 1) << start;
                    int present = bits & mask;
                    int missing = 5 - __builtin_popcount((unsigned)present);
                    if (missing <= 1) {
                        cat = true;
                        break;
                    }
                }
                if (!cat) {
                    int wheel = (1 << 12) | ((1 << 4) - 1);
                    int present = bits & wheel;
                    int missing = 5 - __builtin_popcount((unsigned)present);
                    if (missing <= 1)
                        cat = true;
                }
            }
        }
        if (cat) {
            table[5] += (seq_t)1;
            continue;
        }

        /* 6 – three‑of‑a‑kind */
        if (!jokerCnt) {
            for (int r = 0; r < RANKS; ++r)
                if (rank_counts[r] == 3) {
                    cat = true;
                    break;
                }
        } else {
            for (int r0 = 0; r0 < RANKS && !cat; ++r0) {
                rank_counts[r0]++; /* joker */
                for (int r = 0; r < RANKS; ++r)
                    if (rank_counts[r] == 3) {
                        cat = true;
                        break;
                    }
                rank_counts[r0]--; /* revert */
            }
        }
        if (cat) {
            table[6] += (seq_t)1;
            continue;
        }

        /* 7 – two pair */
        if (!jokerCnt) {
            int paircnt = 0;
            for (int r = 0; r < RANKS; ++r)
                if (rank_counts[r] == 2)
                    paircnt++;
            if (paircnt == 2)
                cat = true;
        } else {
            for (int r0 = 0; r0 < RANKS && !cat; ++r0) {
                rank_counts[r0]++; /* joker */
                int paircnt = 0;
                for (int r = 0; r < RANKS; ++r)
                    if (rank_counts[r] == 2)
                        paircnt++;
                if (paircnt == 2)
                    cat = true;
                rank_counts[r0]--; /* revert */
            }
        }
        if (cat) {
            table[7] += (seq_t)1;
            continue;
        }

        /* 8 – one pair */
        if (!jokerCnt) {
            for (int r = 0; r < RANKS; ++r)
                if (rank_counts[r] == 2) {
                    cat = true;
                    break;
                }
        } else {
            for (int r0 = 0; r0 < RANKS && !cat; ++r0) {
                rank_counts[r0]++; /* joker */
                for (int r = 0; r < RANKS; ++r)
                    if (rank_counts[r] == 2) {
                        cat = true;
                        break;
                    }
                rank_counts[r0]--; /* revert */
            }
        }
        if (cat) {
            table[8] += (seq_t)1;
            continue;
        }
    }
    built = true;
}

seq_t a(idx_t n)
{
    if (!built)
        build_table();
    if (n < 9)
        return table[(size_t)n];
    return 0;
}
