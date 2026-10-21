/*
 * A057696  Number of letters in n in Brazilian Portuguese (not counting
 *          spaces).
 * https://oeis.org/A057696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = number of letters in the written form of n in Brazilian
 *            Portuguese (spaces omitted)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Letter counts for basic components */
static const unsigned char unit_len[10]  = {4,2,4,4,6,5,4,4,4,4};
static const unsigned char teen_len[10]  = {3,4,4,5,8,6,9,9,7,8};
static const unsigned char tens_len[10]  = {0,0,5,6,8,9,8,7,7,7};
static const unsigned char hund_len[10]  = {0,0,8,9,12,10,10,10,10,10};

/* Scale word lengths (singular; plural adds 1) */
static const unsigned char scale_len[16] = {
    0,  /* 0 – no scale */
    3,  /* 1 – mil */
    7,  /* 2 – milhão */
    7,  /* 3 – bilhão */
    7,  /* 4 – trilhão */
    10, /* 5 – quadrilhão */
    10, /* 6 – quintilhão */
    9,  /* 7 – sextilhão */
    9,  /* 8 – septilhão */
    8,  /* 9 – octilhão */
    8,  /* 10 – nonilhão */
    8,  /* 11 – decilhão */
    8,  /* 12 – undecilhão */
    8,  /* 13 – duodecilhão */
    8,  /* 14 – tredecilhão */
    8   /* 15 – quattuordecilhão */
};

/* Recursive routine that returns the length of the written form of n
   (0 ≤ n < 2^64).  It operates on the language itself, never building strings. */
static seq_t len_num(idx_t n)
{
    /* Units and factorial numbers */
    if (n < 10) return unit_len[(int)n];
    if (n < 20) return teen_len[(int)(n-10)];
    if (n < 100) {
        unsigned int tens = (unsigned int)(n/10);
        unsigned int rest = (unsigned int)(n%10);
        seq_t len = tens_len[tens];
        if (rest) len += 1 + unit_len[rest];   /* 'e' and the unit word */
        return len;
    }
    if (n < 1000) {
        unsigned int h    = (unsigned int)(n/100);
        unsigned int rest = (unsigned int)(n%100);
        if (h == 1) {
            if (rest == 0) return 3;                 /* "cem" */
            return 5 + 1 + len_num(rest);           /* "cento" + 'e' + rest */
        }
        seq_t len = hund_len[h];
        if (rest) len += 1 + len_num(rest);          /* 'e' + rest */
        return len;
    }

    /* Numbers ≥ 1 000 – split into groups of 3 digits (base 1000) */
    idx_t pow = 1;
    int k = 0;                                       /* current group index */
    while (pow <= n / 1000) {                        /* find highest power of 1000 ≤ n */
        pow *= 1000;
        k++;
    }
    idx_t top  = n / pow;     /* most significant group */
    idx_t rest = n % pow;     /* lower part */

    seq_t len = 0;

    if (k == 1) { /* thousand group */
        if (top == 1) len += 3;                          /* "mil" */
        else          len += len_num(top) + 3;           /* "<top> mil" */
    } else if (k < (int)(sizeof(scale_len)/sizeof(scale_len[0]))) {
        seq_t scale_word_len = scale_len[k];
        if (top == 1) {
            len += len_num(top) + scale_word_len;        /* "<top> <scale>"  (e.g., "um milhão") */
        } else {
            len += len_num(top) + scale_word_len + (top > 1 ? 1 : 0);
                                            /* plural adds one letter ('s') */
        }
    } else {
        /* For extremely large n the scale names are unknown – ignore */
        len += len_num(top);
    }

    if (rest != 0) {
        len += 1;                     /* connector 'e' */
        len += len_num(rest);          /* lower part */
    }

    return len;
}

seq_t a(idx_t n)
{
    return len_num(n);
}
