/*
 * A052194  Positive integers sorted by length of name in English and
 *          numerical order.
 * https://oeis.org/A052194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052194
    Positive integers sorted by length of English name (US) and numeric order.
    Implementation: precompute a sufficiently large pool of numbers, sort
    them by (length, value) once, and return the n‑th element directly. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- name length calculator (US nomenclature) ---------- */
static const int len_digit[20] = {
    0, /* dummy for 0 */
    3, /* one      */
    3, /* two      */
    5, /* three    */
    4, /* four     */
    4, /* five     */
    3, /* six      */
    5, /* seven    */
    5, /* eight    */
    4, /* nine     */
    3, /* ten      */
    6, /* eleven   */
    6, /* twelve   */
    8, /* thirteen */
    8, /* fourteen */
    7, /* fifteen  */
    7, /* sixteen  */
    9, /* seventeen */
    8, /* eighteen */
    8  /* nineteen */
};

static const int tens_len[10] = {
    0,0,                 /* dummy, dummy */
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

static const int len_hundred = 7; /* "hundred" */

static const int names_len[] = {
    8, /* thousand  */
    7, /* million   */
    8, /* billion   */
    8, /* trillion  */
    11,/* quadrillion */
    11 /* quintillion  */
};

static const uint64_t bases[] = {
    1000ULL,
    1000000ULL,
    1000000000ULL,
    1000000000000ULL,
    1000000000000000ULL,
    1000000000000000000ULL
};

static int name_len_uint64(uint64_t n) {
    if (n < 20)
        return len_digit[(int)n];
    if (n < 100) {
        int tens  = (int)(n / 10);
        int units = (int)(n % 10);
        return tens_len[tens] + (units ? len_digit[units] : 0);
    }
    if (n < 1000) {
        int hundreds = (int)(n / 100);
        uint64_t rest = n % 100;
        int len = len_digit[hundreds] + len_hundred;
        if (rest == 0)
            return len;
        return len + name_len_uint64(rest);
    }
    /* n >= 1000 */
    for (int i = (int)(sizeof(bases)/sizeof(bases[0])) - 1; i >= 0; --i) {
        if (bases[i] <= n) {
            uint64_t high = n / bases[i];
            uint64_t rest = n % bases[i];
            int len = name_len_uint64(high) + names_len[i];
            if (rest == 0)
                return len;
            return len + name_len_uint64(rest);
        }
    }
    return 0; /* unreachable */
}

/* ---------- structure used for sorting ---------- */
struct Number {
    int  len;
    seq_t val;
};

static int cmp_number(const void *a, const void *b) {
    const struct Number *x = (const struct Number *)a;
    const struct Number *y = (const struct Number *)b;
    if (x->len != y->len)
        return (x->len < y->len) ? -1 : 1;
    if (x->val < y->val) return -1;
    if (x->val > y->val) return 1;
    return 0;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    /* Use a static cache so that the expensive precomputation is done only once. */
    static bool initialized = false;
    static seq_t *sorted_seq = NULL;
    static idx_t seq_size  = 0;

    if (!initialized) {
        /* Limit chosen large enough to contain first 10 000 terms
           even for the biggest seq_t widths. */
        const idx_t LIMIT = 500000;
        struct Number *arr = (struct Number *)malloc(LIMIT * sizeof(struct Number));
        if (!arr) abort();

        for (idx_t i = 0; i < LIMIT; ++i) {
            idx_t  val = i + 1;                     /* positive integers only */
            arr[i].len = name_len_uint64((uint64_t)val);
            arr[i].val = (seq_t)val;
        }

        qsort(arr, LIMIT, sizeof(struct Number), cmp_number);

        sorted_seq = (seq_t *)malloc(LIMIT * sizeof(seq_t));
        if (!sorted_seq) abort();

        for (idx_t i = 0; i < LIMIT; ++i)
            sorted_seq[i] = arr[i].val;

        free(arr);
        seq_size = LIMIT;
        initialized = true;
    }

    /* Safety check – the assigned LIMIT must be larger than any n queried. */
    if (n >= seq_size)
        abort(); /* or handle as needed */
    return sorted_seq[n];
}
