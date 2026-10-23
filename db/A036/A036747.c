/*
 * A036747  The numbers 0-100 in English lexicographic order.
 * https://oeis.org/A036747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the sequence A036747: numbers 0‑100 in English
   lexicographic order.  It builds the ordering on the first call by
   generating the English words for all numbers 0…100, sorting them
   lexicographically, and storing the resulting permutation.  Subsequent
   calls simply look up the pre‑computed mapping. */

#include <stdlib.h>
#include <string.h>

/* English words for the numbers 0–100.  The array index is the numeric
   value, and the string is the conventional English representation
   (hyphenated form for 21–99, space for 100). */
static const char *const NUM_WORD[101] = {
    "zero",      /* 0 */
    "one",       /* 1 */
    "two",       /* 2 */
    "three",     /* 3 */
    "four",      /* 4 */
    "five",      /* 5 */
    "six",       /* 6 */
    "seven",     /* 7 */
    "eight",     /* 8 */
    "nine",      /* 9 */
    "ten",       /* 10 */
    "eleven",    /* 11 */
    "twelve",    /* 12 */
    "thirteen",  /* 13 */
    "fourteen",  /* 14 */
    "fifteen",   /* 15 */
    "sixteen",   /* 16 */
    "seventeen", /* 17 */
    "eighteen",  /* 18 */
    "nineteen",  /* 19 */
    "twenty",    /* 20 */
    "twenty-one",      /* 21 */
    "twenty-two",      /* 22 */
    "twenty-three",    /* 23 */
    "twenty-four",     /* 24 */
    "twenty-five",     /* 25 */
    "twenty-six",      /* 26 */
    "twenty-seven",    /* 27 */
    "twenty-eight",    /* 28 */
    "twenty-nine",     /* 29 */
    "thirty",          /* 30 */
    "thirty-one",           /* 31 */
    "thirty-two",           /* 32 */
    "thirty-three",         /* 33 */
    "thirty-four",          /* 34 */
    "thirty-five",          /* 35 */
    "thirty-six",           /* 36 */
    "thirty-seven",         /* 37 */
    "thirty-eight",         /* 38 */
    "thirty-nine",          /* 39 */
    "forty",               /* 40 */
    "forty-one",            /* 41 */
    "forty-two",            /* 42 */
    "forty-three",          /* 43 */
    "forty-four",           /* 44 */
    "forty-five",           /* 45 */
    "forty-six",            /* 46 */
    "forty-seven",          /* 47 */
    "forty-eight",          /* 48 */
    "forty-nine",           /* 49 */
    "fifty",               /* 50 */
    "fifty-one",            /* 51 */
    "fifty-two",            /* 52 */
    "fifty-three",          /* 53 */
    "fifty-four",           /* 54 */
    "fifty-five",           /* 55 */
    "fifty-six",            /* 56 */
    "fifty-seven",          /* 57 */
    "fifty-eight",          /* 58 */
    "fifty-nine",           /* 59 */
    "sixty",               /* 60 */
    "sixty-one",            /* 61 */
    "sixty-two",            /* 62 */
    "sixty-three",          /* 63 */
    "sixty-four",           /* 64 */
    "sixty-five",           /* 65 */
    "sixty-six",            /* 66 */
    "sixty-seven",          /* 67 */
    "sixty-eight",          /* 68 */
    "sixty-nine",           /* 69 */
    "seventy",              /* 70 */
    "seventy-one",          /* 71 */
    "seventy-two",          /* 72 */
    "seventy-three",        /* 73 */
    "seventy-four",         /* 74 */
    "seventy-five",         /* 75 */
    "seventy-six",          /* 76 */
    "seventy-seven",        /* 77 */
    "seventy-eight",        /* 78 */
    "seventy-nine",         /* 79 */
    "eighty",               /* 80 */
    "eighty-one",           /* 81 */
    "eighty-two",           /* 82 */
    "eighty-three",         /* 83 */
    "eighty-four",          /* 84 */
    "eighty-five",          /* 85 */
    "eighty-six",           /* 86 */
    "eighty-seven",         /* 87 */
    "eighty-eight",         /* 88 */
    "eighty-nine",          /* 89 */
    "ninety",               /* 90 */
    "ninety-one",           /* 91 */
    "ninety-two",           /* 92 */
    "ninety-three",         /* 93 */
    "ninety-four",          /* 94 */
    "ninety-five",          /* 95 */
    "ninety-six",           /* 96 */
    "ninety-seven",         /* 97 */
    "ninety-eight",         /* 98 */
    "ninety-nine",          /* 99 */
    "one hundred"           /* 100 */
};

/* Comparator for sorting indices according to the corresponding
   English words. */
static int cmp_indices(const void *p, const void *q)
{
    int i = *(const int *)p;
    int j = *(const int *)q;
    return strcmp(NUM_WORD[i], NUM_WORD[j]);
}

seq_t a(idx_t n)
{
    /* Static mapping from lexicographic rank to numeric value. */
    static idx_t rank_to_num[101];
    static int initialized = 0;

    if (!initialized) {
        int indices[101];
        for (int i = 0; i <= 100; ++i)
            indices[i] = i;

        qsort(indices, 101, sizeof(int), cmp_indices);

        for (int i = 0; i <= 100; ++i)
            rank_to_num[i] = (idx_t)indices[i];

        initialized = 1;
    }

    if (n >= 101)
        return (seq_t)0;          /* Undefined beyond 100; return 0. */

    return (seq_t)rank_to_num[(size_t)n];
}
