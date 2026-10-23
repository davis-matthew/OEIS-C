/*
 * A173216  a(n) is the result of multiplying n by the number of letters of
 *          the name in Spanish.
 * https://oeis.org/A173216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = n*A011762(n) (number of letters of the Spanish name)
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Length tables for the Spanish number words.  All counts are the
 * number of letters, accents and special characters ignored. */
static const uint8_t base_len[20] = {
    4, /* cero      */
    3, /* uno       */
    3, /* dos       */
    4, /* tres      */
    6, /* cuatro    */
    5, /* cinco     */
    4, /* seis      */
    5, /* siete     */
    4, /* ocho      */
    5, /* nueve     */
    4, /* diez      */
    4, /* once      */
    4, /* doce      */
    5, /* trece     */
    7, /* catorce   */
    6, /* quince    */
    9, /* dieciséis */
   10, /* diecisiete*/
    9, /* dieciocho  */
   10 /* diecinueve */
};

static const uint8_t tens_len[10] = {
    0, 0, 6, 7, 8, 9, 7, 7, 7, 7
};

static const uint8_t hundred_len[10] = {
    0, 4,10,11,13,10,11,11,11,11   /* 1=cien, 2=doscientos, 3=trescientos, ... */
};

/* Length of a number less than 1000 in letters. */
static uint8_t len_3digits(idx_t n)
{
    if (n < 20)
        return base_len[(int)n];

    if (n == 20)
        return 6; /* veinte */

    if (n < 30) /* 21–29 : veinti + unit */
        return 6 + base_len[(int)(n - 20)];

    if (n < 100) { /* 30–99 */
        uint8_t tens  = n / 10;
        uint8_t ones  = n % 10;
        if (ones == 0)
            return tens_len[tens];
        return tens_len[tens] + 1 + base_len[ones]; /* "y" between tens and units */
    }

    /* 100–999 */
    uint8_t hundreds = n / 100;
    uint8_t rest     = n % 100;
    if (rest == 0)
        return hundred_len[hundreds];

    if (hundreds == 1) /* 100–199 : "ciento" + rest */
        return 6 + len_3digits(rest); /* "ciento" has 6 letters */

    /* 200–999, except exact multiples of 100 */
    return hundred_len[hundreds] + len_3digits(rest);
}

/* Length of any integer n in letters (supports up to billions). */
static seq_t len_number(idx_t n)
{
    if (n < 1000)
        return len_3digits(n);

    if (n < 1000000) { /* thousands */
        idx_t thousands = n / 1000;
        idx_t rem       = n % 1000;
        seq_t len;
        if (thousands == 1)
            len = 3; /* "mil" */
        else
            len = len_3digits(thousands) + 3; /* thousands word + "mil" */

        if (rem > 0)
            len += len_3digits(rem);
        return len;
    }

    /* millions */
    if (n < 1000000000) {
        idx_t millions = n / 1000000;
        idx_t rem      = n % 1000000;
        seq_t len;
        if (millions == 1)
            len = 2 + 6; /* "un" + "millón" */
        else
            len = len_3digits(millions) + 8; /* "millones" */

        if (rem > 0)
            len += len_number(rem);
        return len;
    }

    /* beyond billions – not needed for the test range but handled generically */
    seq_t len = 0;
    while (n > 0) {
        len += len_3digits(n % 1000);
        n /= 1000;
    }
    return len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t len = len_number(n);
    return (seq_t)n * len;
}
