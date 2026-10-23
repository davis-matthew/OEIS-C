/*
 * A163828  Number of straight line segments in all letters of the capitalized
 *          English name of n.
 * https://oeis.org/A163828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = number of straight line segments in the capitalized
 *            English name of n (with strokes defined as in the definition)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t letter_strokes[26] = {
    3, /* A */
    1, /* B */
    0, /* C */
    1, /* D */
    4, /* E */
    3, /* F */
    2, /* G */
    3, /* H */
    1, /* I */
    1, /* J */
    3, /* K */
    2, /* L */
    4, /* M */
    3, /* N */
    0, /* O */
    1, /* P */
    1, /* Q */
    2, /* R */
    0, /* S */
    2, /* T */
    0, /* U */
    2, /* V */
    4, /* W */
    2, /* X */
    3, /* Y */
    3  /* Z */
};

/* Names used in the English number representation. */
static const char *small_names[20] = {
    "ZERO",   /* 0 */
    "ONE",    /* 1 */
    "TWO",    /* 2 */
    "THREE",  /* 3 */
    "FOUR",   /* 4 */
    "FIVE",   /* 5 */
    "SIX",    /* 6 */
    "SEVEN",  /* 7 */
    "EIGHT",  /* 8 */
    "NINE",   /* 9 */
    "TEN",    /* 10 */
    "ELEVEN", /* 11 */
    "TWELVE", /* 12 */
    "THIRTEEN",/* 13 */
    "FOURTEEN",/* 14 */
    "FIFTEEN",/* 15 */
    "SIXTEEN",/* 16 */
    "SEVENTEEN",/* 17 */
    "EIGHTEEN",/* 18 */
    "NINETEEN" /* 19 */
};

static const char *tens_names[8] = {
    "TWENTY",   /* 20 */
    "THIRTY",   /* 30 */
    "FORTY",    /* 40 */
    "FIFTY",    /* 50 */
    "SIXTY",    /* 60 */
    "SEVENTY",  /* 70 */
    "EIGHTY",   /* 80 */
    "NINETY"    /* 90 */
};

static const char *scale_names[7] = {
    "",            /* units */
    "THOUSAND",    /* thousand */
    "MILLION",     /* million */
    "BILLION",     /* billion */
    "TRILLION",    /* trillion */
    "QUADRILLION", /* quadrillion */
    "QUINTILLION"  /* quintillion */
};

/* Sum the strokes for a single word. */
static seq_t word_strokes(const char *w) {
    seq_t sum = 0;
    for (; *w != '\0'; ++w) {
        char c = *w;
        if (c >= 'A' && c <= 'Z')
            sum += letter_strokes[c - 'A'];
    }
    return sum;
}

/* Compute strokes for a three‑digit block (0 ≤ t ≤ 999). */
static seq_t strokes_triplet(uint16_t t) {
    seq_t sum = 0;

    if (t == 0)
        return 0; /* zero is handled outside */

    if (t >= 100) {
        uint16_t h = t / 100;
        sum += word_strokes(small_names[h]);   /* digit name */
        sum += word_strokes("HUNDRED");
    }

    uint16_t rem = t % 100;
    if (rem >= 20) {
        uint16_t tens = rem / 10;
        uint16_t unit = rem % 10;
        sum += word_strokes(tens_names[tens - 2]); /* tens_names[0] is TWENTY */
        if (unit)
            sum += word_strokes(small_names[unit]);
    } else if (rem) {
        sum += word_strokes(small_names[rem]);
    }

    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return word_strokes("ZERO");

    seq_t total = 0;
    idx_t tmp = n;
    int group_index = 0;   /* 0 → units, 1 → thousand, etc. */

    while (tmp > 0) {
        uint16_t group = (uint16_t)(tmp % 1000);
        if (group) {
            total += strokes_triplet(group);
            if (group_index > 0)
                total += word_strokes(scale_names[group_index]);
        }
        tmp /= 1000;
        ++group_index;
    }

    return total;
}
