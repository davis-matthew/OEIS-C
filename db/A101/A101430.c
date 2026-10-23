/*
 * A101430  a(1)=1 and a(n+1) = n + (n-th digit of the sequence).
 * https://oeis.org/A101430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n+1) = n + (n-th digit of the sequence), a(1)=1
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper functions */
static void ensure_terms_capacity(idx_t needed);
static void ensure_digits_capacity(idx_t needed);
static void append_digits(seq_t val);

/* Storage for computed terms and digits */
static idx_t term_count = 0;      /* number of terms stored (a(1)..a(term_count)) */
static seq_t *terms = NULL;       /* 1‑indexed array, terms[0] unused */
static idx_t terms_size = 0;

static idx_t digit_count = 0;     /* number of digits stored (concatenated sequence) */
static char *digits = NULL;       /* array of chars '0'..'9' */
static idx_t digits_size = 0;

/* Initialisation on first call */
static void init_storage(void) {
    if (terms != NULL) return;       /* already initialised */
    /* initial capacities */
    terms_size = 16;
    terms = (seq_t *)malloc(terms_size * sizeof(seq_t));
    digits_size = 256;
    digits = (char *)malloc(digits_size * sizeof(char));
    /* a(1) = 1 */
    terms[1] = (seq_t)1;
    term_count = 1;
    /* its digit '1' */
    digits[0] = '1';
    digit_count = 1;
}

static void ensure_terms_capacity(idx_t needed) {
    if (needed <= terms_size) return;
    idx_t new_size = terms_size ? terms_size << 1 : 16;
    while (new_size < needed) new_size <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
    if (!new_ptr) exit(1);
    terms = new_ptr;
    terms_size = new_size;
}

static void ensure_digits_capacity(idx_t needed) {
    if (needed <= digits_size) return;
    idx_t new_size = digits_size ? digits_size << 1 : 256;
    while (new_size < needed) new_size <<= 1;
    char *new_ptr = (char *)realloc(digits, new_size * sizeof(char));
    if (!new_ptr) exit(1);
    digits = new_ptr;
    digits_size = new_size;
}

static void append_digits(seq_t val) {
    /* Temporary buffer: max 40 digits for uint128 */
    char buf[40];
    idx_t len = 0;
    if (val == (seq_t)0) {
        buf[len++] = '0';
    } else {
        while (val > (seq_t)0) {
            seq_t d = val % (seq_t)10;
            buf[len++] = '0' + (int)d;
            val = val / (seq_t)10;
        }
    }
    ensure_digits_capacity(digit_count + len);
    for (idx_t i = 0; i < len; ++i) {
        digits[digit_count++] = buf[len - 1 - i];
    }
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    init_storage();
    if (n > term_count) {
        for (idx_t i = term_count + 1; i <= n; ++i) {
            idx_t prev = i - 1;                       /* n in recurrence */
            char dchar = digits[prev - 1];            /* previous digit (0‑based) */
            seq_t digit = (seq_t)(dchar - '0');
            seq_t val = (seq_t)prev + digit;          /* a(i) */
            ensure_terms_capacity(i + 1);
            terms[i] = val;
            append_digits(val);
        }
        term_count = n;
    }
    return terms[n];
}
