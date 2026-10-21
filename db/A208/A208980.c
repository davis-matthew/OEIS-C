/*
 * A208980  List of winning positions in Barry Cipra's Factor Subtractor game.
 * https://oeis.org/A208980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 9107, u32 n <= 9107, u64 n <= 9107,
 *            u128 n <= 9107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A208980: a(n) is the nth winning
   number in the Factor Subtractor game.  A number `x` is winning iff it
   has a proper factorization `x = h * k` (with `h,k > 1`) and both
   `x - h` and `x - k` are not winning. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Returns the n-th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool initialized = false;
    static idx_t nextToTest = 2;          /* next integer to examine */
    static bool *isWin = NULL;            /* array marking winning numbers */
    static size_t isWinCap = 0;
    static seq_t *wins = NULL;            /* array of winning numbers, sorted */
    static size_t winsSize = 0, winsCap = 0;

    /* Initialisation */
    if (!initialized) {
        isWinCap = 16;                    /* initial capacity for isWin */
        isWin = (bool*)malloc(isWinCap * sizeof(bool));
        if (!isWin) return (seq_t)0;      /* out of memory */
        for (size_t i = 0; i < isWinCap; ++i) isWin[i] = false;

        winsCap = 16;                     /* initial capacity for wins */
        wins = (seq_t*)malloc(winsCap * sizeof(seq_t));
        if (!wins) return (seq_t)0;       /* out of memory */
        winsSize = 0;

        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (winsSize < n) {
        idx_t i = nextToTest++;
        /* Ensure the isWin array is large enough for index i */
        if ((size_t)i >= isWinCap) {
            size_t newCap = isWinCap;
            while ((size_t)i >= newCap) newCap <<= 1;
            bool *tmp = (bool*)realloc(isWin, newCap * sizeof(bool));
            if (!tmp) return (seq_t)0;
            /* Initialize new entries to false */
            for (size_t j = isWinCap; j < newCap; ++j) tmp[j] = false;
            isWin = tmp;
            isWinCap = newCap;
        }

        bool winFlag = false;  /* whether i is a winning position */

        if (i >= 4) {          /* only numbers ≥4 can have proper factorizations */
            for (idx_t d = 2; d <= i / d; ++d) {   /* test divisors up to √i */
                if (i % d == 0) {
                    idx_t h = d, k = i / d;        /* proper factors */
                    /* Both i-h and i-k must be non‑winning */
                    if (!isWin[(size_t)(i - h)] && !isWin[(size_t)(i - k)]) {
                        winFlag = true;
                        break;
                    }
                }
            }
        }

        isWin[(size_t)i] = winFlag;   /* store result for future lookups */

        if (winFlag) {
            /* Append i to the array of winning numbers */
            if (winsSize == winsCap) {
                size_t newCap = winsCap << 1;
                seq_t *tmp = (seq_t*)realloc(wins, newCap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                wins = tmp;
                winsCap = newCap;
            }
            wins[winsSize++] = (seq_t)i;
        }
    }

    /* Return the n-th winning number (1‑based index) */
    return wins[n - 1];
}
