/*
 * A124060  T totaler sequence. Number of t's in the American spelling of
 *          numbers.
 * https://oeis.org/A124060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count 't's in the American English word representation of n
   (offset 0: a(0)=0).  The routine uses tables of precomputed
   't' counts for the units, tens, and scale words.  It works for
   all non‑negative n that fit in the idx_t type. */
#include <stdint.h>

static const unsigned char smallCount[20] = {
    0, /* 0 zero   */
    0, /* 1 one    */
    1, /* 2 two    */
    1, /* 3 three  */
    0, /* 4 four   */
    0, /* 5 five   */
    0, /* 6 six    */
    0, /* 7 seven  */
    1, /* 8 eight  */
    0, /* 9 nine   */
    1, /*10 ten    */
    0, /*11 eleven */
    1, /*12 twelve */
    2, /*13 thirteen */
    1, /*14 fourteen */
    1, /*15 fifteen  */
    1, /*16 sixteen  */
    1, /*17 seventeen */
    1, /*18 eighteen  */
    1  /*19 nineteen  */
};

static const unsigned char tensCount[10] = {
    0, /* 0 unused */
    0, /* 1 unused */
    2, /* 2 twenty */
    2, /* 3 thirty */
    1, /* 4 forty  */
    1, /* 5 fifty  */
    1, /* 6 sixty  */
    1, /* 7 seventy */
    1, /* 8 eighty  */
    1  /* 9 ninety  */
};

static const idx_t scale_pow[6] = {
    1000ULL,                 /* thousand     */
    1000000ULL,              /* million      */
    1000000000ULL,           /* billion      */
    1000000000000ULL,        /* trillion     */
    1000000000000000ULL,     /* quadrillion  */
    1000000000000000000ULL   /* quintillion  */
};

static const unsigned char scale_t_cnt[6] = {
    1, /* thousand     */
    0, /* million      */
    0, /* billion      */
    1, /* trillion     */
    0, /* quadrillion  */
    1  /* quintillion  */
};

/* Count the 't's in the words for numbers 1 .. 999 (no leading zeros). */
static seq_t count_1_to_999(uint32_t n)
{
    if (n < 20)
        return (seq_t)smallCount[n];

    if (n < 100) {
        unsigned tens = n / 10;
        unsigned ones = n % 10;
        return (seq_t)tensCount[tens] + (ones ? (seq_t)smallCount[ones] : 0);
    }

    /* 100 <= n < 1000 */
    unsigned hundreds = n / 100;
    unsigned rem = n % 100;
    seq_t res = (seq_t)smallCount[hundreds];
    if (rem)
        res += count_1_to_999(rem);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;

    for (int i = 5; i >= 0; --i) {
        idx_t pow = scale_pow[i];
        if (n >= pow) {
            idx_t group = n / pow;
            n %= pow;
            total += count_1_to_999((uint32_t)group);
            total += scale_t_cnt[i];
        }
    }

    if (n > 0)
        total += count_1_to_999((uint32_t)n);

    return total;
}
