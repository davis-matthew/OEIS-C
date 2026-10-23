/*
 * A170835  Most common to least common letters in the "QWERTY" keyboard
 *          (numbers labeled).
 * https://oeis.org/A170835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(n) = the key number of the nth most frequent alphabetic key on
 *            * a QWERTY keyboard, where the letters are ranked by relative
 *            frequency. * The alphabetic keys are numbered 1..26 as: Q=1,
 *            W=2, E=3, R=4, T=5, Y=6, * U=7, I=8, O=9, P=10, A=11, S=12,
 *            D=13, F=14, G=15, H=16, J=17, K=18, * L=19, Z=20,...
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1 || n > 26) return (seq_t)0;

    /* Frequency values (scaled to preserve the exact ranking of the
     * OEIS entry).  The values are chosen so that a descending sort
     * of the key numbers reproduces the sequence:
     * 3,5,11,9,8,25,12,4,16,19,13,22,7,26,14,10,15,2,6,24,23,18,21,17,1,20
     */
    static const int freq[26] = {
        2,  /* Q  (key 1)      */
        9,  /* W  (2)           */
        26, /* E  (3)           */
        19, /* R  (4)           */
        25, /* T  (5)           */
        8,  /* Y  (6)           */
        14, /* U  (7)           */
        22, /* I  (8)           */
        23, /* O  (9)           */
        11, /* P  (10)          */
        24, /* A  (11)          */
        20, /* S  (12)          */
        16, /* D  (13)          */
        12, /* F  (14)          */
        10, /* G  (15)          */
        18, /* H  (16)          */
        3,  /* J  (17)          */
        5,  /* K  (18)          */
        17, /* L  (19)          */
        1,  /* Z  (20)          */
        4,  /* X  (21)          */
        15, /* C  (22)          */
        6,  /* V  (23)          */
        7,  /* B  (24)          */
        21, /* N  (25)          */
        13  /* M  (26)          */
    };

    /* Static storage for the sorted key numbers. */
    static idx_t sorted_keys[26];
    static bool initialized = false;

    if (!initialized) {
        /* Initialise sorted_keys with 0..25 (indices of freq array). */
        for (idx_t i = 0; i < 26; ++i)
            sorted_keys[i] = i;

        /* Sort indices descending by frequency using insertion sort. */
        for (idx_t i = 1; i < 26; ++i) {
            idx_t key_idx = sorted_keys[i];
            int   key_val = freq[key_idx];
            idx_t j = i;
            while (j > 0 && freq[sorted_keys[j - 1]] < key_val) {
                sorted_keys[j] = sorted_keys[j - 1];
                --j;
            }
            sorted_keys[j] = key_idx;
        }

        initialized = true;
    }

    /* Map sorted index to key number (1‑based) and cast to seq_t. */
    return (seq_t)(sorted_keys[n - 1] + 1);
}
