/*
 * A226911  Remainder modulo n of the sum of the letters of the English
 *          word(s) for n (A073327: a=1, ..., z=26).
 * https://oeis.org/A226911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, look
 * Formula:   a(n) = A073327(n) mod n (sum of letter values of the English
 *            word for n)
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Letter values of the basic number words --------------------------------- */
static const seq_t sum_units[10] = {
    0,   /* 0 (unused) */
    34,  /* one   */
    58,  /* two   */
    56,  /* three */
    60,  /* four  */
    42,  /* five  */
    52,  /* six   */
    65,  /* seven */
    49,  /* eight */
    42   /* nine  */
};

static const seq_t sum_teens[10] = {
    39,  /* ten     */
    63,  /* eleven  */
    87,  /* twelve  */
    99,  /* thirteen*/
    104, /* fourteen*/
    65,  /* fifteen */
    96,  /* sixteen */
    109, /* seventeen */
    73,  /* eighteen */
    86   /* nineteen */
};

static const seq_t sum_tens[10] = {
    0,   /* 0 */
    0,   /* 10s placeholder */
    107, /* twenty  */
    100, /* thirty  */
    84,  /* forty   */
    66,  /* fifty   */
    97,  /* sixty   */
    110, /* seventy */
    74,  /* eighty  */
    87   /* ninety  */
};

/* Scale word sums --------------------------------------------------------- */
static const seq_t sum_hundred   = 74;   /* hundred  */
static const seq_t sum_thousand  = 102;  /* thousand */
static const seq_t sum_million   = 75;   /* million  */
static const seq_t sum_billion   = 73;   /* billion  */
static const seq_t sum_trillion  = 109;  /* trillion */
static const seq_t sum_quadrillion = 132;/* quadrillion*/
static const seq_t sum_quintillion = 167;/* quintillion*/

static const seq_t scale_word_sums[] = {
    0,                  /* no scale for the units block   */
    sum_thousand,       /* thousands                       */
    sum_million,        /* millions                        */
    sum_billion,        /* billions                        */
    sum_trillion,       /* trillions                       */
    sum_quadrillion,    /* quadrillions                    */
    sum_quintillion     /* quintillions                    */
};

/* ------------------------------------------------------------------------- */
/* Convert a 3‑digit group (0 < g < 1000) to its letter sum -----------------
   Note: for g = 0 the function returns 0 and the caller ignores empty
   blocks. ---------------------------------------------------------------- */
static seq_t group_letter_sum(idx_t g)
{
    seq_t total = 0;

    if (g >= 100)
    {
        total += sum_units[(size_t)(g / 100)] + sum_hundred;
        g %= 100;
    }

    if (g >= 20)
    {
        total += sum_tens[(size_t)(g / 10)];
        g %= 10;
        if (g > 0)
            total += sum_units[(size_t)g];
    }
    else if (g >= 10)
    {
        total += sum_teens[(size_t)(g - 10)];
    }
    else if (g > 0)
    {
        total += sum_units[(size_t)g];
    }

    return total;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety guard for n = 0 (outside offset) */

    /* Break n into groups of three digits (units, thousands, …) */
    idx_t group[10];
    size_t groups = 0;
    idx_t temp = n;
    while ((temp > 0) && (groups < sizeof(group)/sizeof(group[0])))
    {
        group[groups++] = temp % 1000;
        temp /= 1000;
    }

    seq_t total = 0;

    /* Process from the most significant group to the least */
    for (size_t i = groups; i-- > 0; )
    {
        if (group[i] == 0)
            continue;                 /* skip empty block */

        total += group_letter_sum(group[i]);   /* word(s) for the group */

        if (i > 0 && i < sizeof(scale_word_sums)/sizeof(scale_word_sums[0]))
            total += scale_word_sums[i];       /* add the scale word */
    }

    return total % (seq_t)n;
}
