/*
 * A072685  Positive integers whose English names contain a prime number of
 *          letters.
 * https://oeis.org/A072685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the n‑th positive integer whose English
 * representation (with British conventions, words concatenated
 * without spaces or hyphens) contains a prime number of letters.
 * The implementation simply scans integer values in order, counts
 * the word length, and records the number when the count is prime.
 * This avoids dynamic allocation and works for both plain integer
 * seq_t and for an arbitrary‑precision seq_t class compiled with
 * C++. The function is O(n) in practice for n up to about 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_len.h"

/* Letter counts for units 1–19 (index 0 unused; 20 handled separately). */
static const int unit_len[20] = {
    0,          /* 0 */
    3,          /* 1  ONE */
    3,          /* 2  TWO */
    5,          /* 3  THREE */
    4,          /* 4  FOUR */
    4,          /* 5  FIVE */
    3,          /* 6  SIX */
    5,          /* 7  SEVEN */
    5,          /* 8  EIGHT */
    4,          /* 9  NINE */
    3,          /*10  TEN */
    6,          /*11  ELEVEN */
    6,          /*12  TWELVE */
    8,          /*13  THIRTEEN */
    8,          /*14  FOURTEEN */
    7,          /*15  FIFTEEN */
    7,          /*16  SIXTEEN */
    9,          /*17  SEVENTEEN */
    8,          /*18  EIGHTEEN */
    8           /*19  NINETEEN */
};

/* Letter counts for tens 20–90 (indices 2–9). */
static const int tens_len[10] = {
    0, 0,          /* 0,10 */
    6, 6,          /* 20 TWENTY, 30 THIRTY */
    5, 5,          /* 40 FORTY, 50 FIFTY */
    5, 7,          /* 60 SIXTY, 70 SEVENTY */
    6, 6           /* 80 EIGHTY, 90 NINETY */
};

/* Lengths of scale words: index 1 = THOUSAND, 2 = MILLION, etc. */
static const int scale_len[10] = {
    0, 8, 7, 7, 8, 7, 8, 7, 8, 7
};

/* Count letters in a 1…999 group (e.g., 342 is "THREEHUNDREDANDFOURTY…"). */
static int letters_group(idx_t g)
{
    int total = 0;
    int hundreds = (int)(g / 100);
    int rem = (int)(g % 100);
    if (hundreds) {
        total += unit_len[hundreds];   /* number before HUNDRED */
        total += 7;                    /* HUNDRED */
        if (rem)                      /* remainder after hundreds */
            total += 3;                /* AND */
    }
    if (rem >= 20) {
        int tens = rem / 10;
        int unit = rem % 10;
        total += tens_len[tens];
        if (unit)
            total += unit_len[unit];
    } else if (rem) {
        total += unit_len[rem];
    }
    return total;
}

/* Count letters in the full English name of n >= 1 (British convention). */
static int letters_len(idx_t n)
{
    if (n < 20)
        return unit_len[(int)n];

    int total = 0;
    idx_t rem = n;
    int group_idx = 0;          /* 0 = units, 1 = thousands, ... */
    while (rem > 0) {
        uint32_t group = (uint32_t)(rem % 1000);
        if (group) {
            int part = letters_group(group);
            if (group_idx > 0)
                part += scale_len[group_idx];
            total += part;
        }
        rem /= 1000;
        group_idx++;
    }
    /* British rule: add AND between the highest scale word and the
     * lower part if that lower part is < 100 and non‑zero. */
    if (n >= 1000) {
        uint32_t low = (uint32_t)(n % 1000);
        if (low != 0 && low < 100)
            total += 3;         /* AND */
    }
    return total;
}

/* Main OEIS function a(n).  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* guard against zero, though offset is 1 */

    idx_t count = 0;
    idx_t num = 1;
    while (count < n) {
        int len = letters_len(num);
        if (is_prime_len(len)) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
        num++;
    }
    return 0;                   /* never reached for valid n */
}
