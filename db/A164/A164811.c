/*
 * A164811  a(n) = smallest number which has in its Spanish name the letter
 *          "a" in the n-th position, or -1 if no such number exists.
 * https://oeis.org/A164811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, sign
 * Formula:   a(n) = smallest number whose Spanish name has the letter “a” at
 *            the n‑th position, or –1 if no such number exists. The value is
 *            obtained by a forward search over natural numbers with a
 *            light‑weight Spanish number name generator
 * Range:     i8 n <= 4, i16 n <= 35, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Helper arrays for Spanish number words (no accents, no punctuation). */
static const char *const units[20] = {
    "cero","uno","dos","tres","cuatro","cinco","seis","siete","ocho","nueve",
    "diez","once","doce","trece","catorce","quince","dieciseis",
    "diecisiete","dieciocho","diecinueve"
};

static const char *const tens[10] = {
    "", "", "veinte","treinta","cuarenta","cincuenta","sesenta","setenta",
    "ochenta","noventa"
};

static const char *const hundreds[10] = {
    "", "", "doscientos","trescientos","cuatrocientos","quinientos",
    "seiscientos","setecientos","ochocientos","novecientos"
};

/* Append all alphabetic characters of a word to a buffer, converted to lowercase. */
static void append_word(const char *word, char *buf, size_t *len)
{
    for (; *word; ++word) {
        char c = *word;
        if (isalpha((unsigned char)c))
            buf[(*len)++] = (char)tolower((unsigned char)c);
    }
}

/* Recursively build the Spanish name of n into buf.  len is updated. */
static void build_name(uint64_t n, char *buf, size_t *len)
{
    if (n >= 1000000ULL) {
        uint64_t m = n / 1000000ULL;
        uint64_t r = n % 1000000ULL;
        if (m == 1ULL) {
            append_word("un", buf, len);
            append_word("millon", buf, len);          /* “millón” without accent */
        } else {
            build_name(m, buf, len);
            append_word("millones", buf, len);
        }
        if (r > 0) build_name(r, buf, len);
        return;
    }
    if (n >= 1000ULL) {
        uint64_t t = n / 1000ULL;
        uint64_t r = n % 1000ULL;
        if (t == 1ULL) {
            append_word("mil", buf, len);
        } else {
            build_name(t, buf, len);
            append_word("mil", buf, len);
        }
        if (r > 0) build_name(r, buf, len);
        return;
    }
    if (n >= 100ULL) {
        if (n == 100ULL) {
            append_word("cien", buf, len);
            return;
        }
        uint64_t h = n / 100ULL;
        uint64_t r = n % 100ULL;
        if (h == 1ULL) {
            append_word("ciento", buf, len);
        } else {
            append_word(hundreds[(int)h], buf, len);
        }
        if (r > 0) build_name(r, buf, len);
        return;
    }
    if (n >= 30ULL) {
        uint64_t t = n / 10ULL;
        uint64_t u = n % 10ULL;
        append_word(tens[(int)t], buf, len);
        if (u > 0) {
            append_word("y", buf, len);
            build_name(u, buf, len);
        }
        return;
    }
    if (n >= 20ULL) {
        if (n == 20ULL) {
            append_word("veinte", buf, len);
            return;
        }
        append_word("veinti", buf, len);
        build_name(n - 20ULL, buf, len);
        return;
    }
    /* 0 to 19 */
    append_word(units[(int)n], buf, len);
}

/* Ensure the answer table can hold indices up to n (1‑based).  Missing positions are
   initially set to (seq_t)-1. */
static void ensure_capacity(idx_t n)
{
    static seq_t *answers = NULL;
    static size_t cap = 0;

    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* On allocation failure fall back to maximum‑possible answer */
            for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)-1;
            answers = tmp;
            cap = new_cap;
            return;
        }
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)-1;
        answers = tmp;
        cap = new_cap;
    }
    /* Expose via static function pointer for the next routine */
    static seq_t **ans_ptr = &answers;
    *ans_ptr = answers; /* just to keep the compiler from complaining */
    (void)ans_ptr;
}

/* Forward declaration of the main search routine so we can embed the static data. */
static seq_t search_for_n(idx_t n);

/* Global data used by the search routine. */
static seq_t *answers = NULL;
static size_t answers_cap = 0;
static uint64_t next_num = 1;   /* next natural number to process */

/* Helper to ensure capacity for given n. */
static void answers_ensure_idx(idx_t n)
{
    if ((size_t)n > answers_cap) {
        size_t new_cap = answers_cap ? answers_cap : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – but we cannot recover, return */
            return;
        }
        for (size_t i = answers_cap; i < new_cap; ++i) tmp[i] = (seq_t)-1;
        answers = tmp;
        answers_cap = new_cap;
    }
}

/* Search until we determine the answer for index n.  This function
   updates the global answers array. */
static seq_t search_for_n(idx_t n)
{
    const uint64_t MAX_SCAN = 2000000ULL;   /* safety limit to avoid an endless loop */
    char buf[512];
    size_t len;
    while (next_num <= MAX_SCAN) {
        len = 0;
        build_name(next_num, buf, &len);
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 'a') {
                idx_t pos = (idx_t)(i + 1);   /* 1‑based position in the word */
                answers_ensure_idx(pos);
                if (answers[pos - 1] == (seq_t)-1)
                    answers[pos - 1] = (seq_t)next_num;
            }
        }
        if (n <= len && answers[n - 1] != (seq_t)-1)
            return answers[n - 1];
        ++next_num;
    }
    /* If we exit the loop without finding an answer, no such number exists within the
       searched range – we assume the sequence has no entry for this index. */
    answers_ensure_idx(n);
    return answers[n - 1];
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;          /* undefined index, safety fallback */

    answers_ensure_idx(n);
    if (answers[n - 1] != (seq_t)-1)
        return answers[n - 1];

    return search_for_n(n);
}
