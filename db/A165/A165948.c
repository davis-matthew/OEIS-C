/*
 * A165948  Base-10 encoding of the Spanish name of n with one digit per
 *          letter as on a touch-tone telephone.
 * https://oeis.org/A165948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, base
 * Formula:   a(n) = digit sequence obtained by typing the Spanish name of n
 *            on a telephone keypad (a,b,c→2, d,e,f→3, g,h,i→4, j,k,l→5,
 *            m,n,o→6, p,q,r,s→7, t,u,v→8, w,x,y,z→9)
 * Range:     u8 none, u16 n <= 3, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void append_str(const char *s, char **p) {
    for (; *s; ++*p, ++s) **p = *s;
}

static const char *w_0_16[17] = {
    "cero","uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve",
    "diez","once","doce","trece","catorce","quince","dieciseis"
};

static const char *w_17_to_19[3] = {
    "diecisiete","dieciocho","diecinueve"
};

static const char *veinti_suffix[9] = {
    "uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve"
};

static const char *tens_names[10] = {
    "","", "", "treinta", "cuarenta", "cincuenta",
    "sesenta", "setenta", "ochenta", "noventa"
};

static const char *units[10] = {
    "", "uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve"
};

static const char *hundreds_names[10] = {
    "", "", "doscientos", "trescientos", "cuatrocientos",
    "quinientos", "seiscientos", "setecientos",
    "ochocientos", "novecientos"
};

static void word_less_1000(idx_t n, char **p) {
    if (n < 20) {
        if (n <= 16) {
            append_str(w_0_16[n], p);
            return;
        } else {                  /* 17,18,19 */
            append_str(w_17_to_19[n-17], p);
            return;
        }
    }
    if (n < 30) {
        if (n == 20) {
            append_str("veinte", p);
            return;
        }
        /* 21–29 */
        append_str("veinti", p);
        append_str(veinti_suffix[n-21], p);
        return;
    }
    if (n < 100) {
        int tens = n/10;
        int unit = n%10;
        if (unit == 0) {
            append_str(tens_names[tens], p);
        } else {
            append_str(tens_names[tens], p);
            append_str("y", p);
            append_str(units[unit], p);
        }
        return;
    }
    /* 100–999 */
    int hundreds = n/100;
    int rest = n%100;
    if (hundreds == 1) {
        if (rest == 0) {
            append_str("cien", p);
            return;
        } else {
            append_str("ciento", p);
            word_less_1000(rest, p);
            return;
        }
    }
    append_str(hundreds_names[hundreds], p);
    if (rest > 0) word_less_1000(rest, p);
}

static void word_of_n(idx_t n, char *buf) {
    char *p = buf;
    if (n == 0) {
        append_str("cero", &p);
        *p = '\0';
        return;
    }
    if (n < 1000) {
        word_less_1000(n, &p);
        *p = '\0';
        return;
    }
    idx_t thousand = n/1000;
    idx_t rest = n%1000;
    if (thousand == 1) {
        append_str("mil", &p);
    } else {
        word_less_1000(thousand, &p);
        append_str("mil", &p);
    }
    if (rest != 0) word_less_1000(rest, &p);
    *p = '\0';
}

static const unsigned char digit_map[26] = {
    2,2,2, /* a,b,c */
    3,3,3, /* d,e,f */
    4,4,4, /* g,h,i */
    5,5,5, /* j,k,l */
    6,6,6, /* m,n,o */
    7,7,7,7, /* p,q,r,s */
    8,8,8, /* t,u,v */
    9,9,9,9 /* w,x,y,z */
};

seq_t a(idx_t n) {
    char word[512];
    word_of_n(n, word);
    seq_t res = 0;
    for (char *s = word; *s; ++s) {
        seq_t d = digit_map[(unsigned char)(*s - 'a')];
        res = res * 10 + d;
    }
    return res;
}
