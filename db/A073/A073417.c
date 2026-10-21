/*
 * A073417  Flawless numbers: numbers without a, f, l or w.
 * https://oeis.org/A073417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 336, u32 n <= 336, u64 n <= 336,
 *            u128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flawless numbers: numbers whose English name contains no letters a, f, l, or w.
   The sequence is finite: a(336) = 999.  This implementation builds a lookup table
   of all such numbers up to 999 on first call. */

#include <stdlib.h>
#include <stdbool.h>

/* Masks for forbidden letters: bit0 = 'a', bit1 = 'f', bit2 = 'l', bit3 = 'w'.
   The array holds a mask for each number from 0 to 19 (inclusive). */
static const unsigned char numMask[20] = {
    0,   /* 0  zero   */
    0,   /* 1  one    */
    8,   /* 2  two    (w)    */
    0,   /* 3  three  */
    2,   /* 4  four   (f)    */
    2,   /* 5  five   (f)    */
    0,   /* 6  six    */
    0,   /* 7  seven  */
    0,   /* 8  eight  */
    0,   /* 9  nine   */
    0,   /* 10 ten    */
    4,   /* 11 eleven (l)    */
    12,  /* 12 twelve (l,w)  */
    0,   /* 13 thirteen */
    2,   /* 14 fourteen (f)  */
    2,   /* 15 fifteen  (f)  */
    0,   /* 16 sixteen  */
    0,   /* 17 seventeen */
    0,   /* 18 eighteen  */
    0    /* 19 nineteen  */
};

/* Masks for the tens words 20,30,...,90. */
static const unsigned char tensMask[8] = {
    8,   /* 20 twenty (w)      */
    0,   /* 30 thirty          */
    2,   /* 40 forty (f)       */
    2,   /* 50 fifty (f)       */
    0,   /* 60 sixty           */
    0,   /* 70 seventy         */
    0,   /* 80 eighty          */
    0    /* 90 ninety          */
};

static bool has_forbidden_letters(idx_t n)
{
    unsigned int mask = 0;
    if (n >= 100) {
        idx_t h = n / 100;          /* hundreds digit word */
        mask |= numMask[h];
        n %= 100;                   /* remaining two digits */
    }
    if (n >= 20) {
        idx_t t = n / 10;           /* tens digit word  */
        mask |= tensMask[t - 2];
        n %= 10;                    /* remaining ones digit */
    }
    if (n > 0) {
        mask |= numMask[n];
    }
    return mask != 0;
}

static seq_t *flawless = NULL;
static idx_t flaw_cnt = 0;

static void init_flawless(void)
{
    if (flawless)
        return;
    flawless = (seq_t *)malloc(1000 * sizeof(seq_t));
    idx_t i = 0;
    for (idx_t n = 0; n <= 999; ++n) {
        if (!has_forbidden_letters(n))
            flawless[i++] = (seq_t)n;
    }
    flaw_cnt = i;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined offset rises to 1 in OEIS */
        return 0;
    init_flawless();
    idx_t pos = n - 1;
    if (pos >= flaw_cnt)
        return 0;      /* sequence ends at a(336) = 999 */
    return flawless[pos];
}
