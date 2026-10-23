/*
 * A108017  Position of integer n in the list of first n natural numbers
 *          written out in English (without "and") and arranged in
 *          alphabetical order.
 * https://oeis.org/A108017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   For a given n, generate the English word (without “and”) for all
 *            numbers 1 … n, sort these words alphabetically, and return the
 *            1‑based position of n in that sorted list
 * Range:     u8 n <= 599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* English components – all uppercase, no spaces or hyphens. */
static const char * const UNITS[] = {
    "ONE", "TWO", "THREE", "FOUR", "FIVE",
    "SIX", "SEVEN", "EIGHT", "NINE"
};

static const char * const TEENS[] = {
    "TEN", "ELEVEN", "TWELVE", "THIRTEEN",
    "FOURTEEN", "FIFTEEN", "SIXTEEN",
    "SEVENTEEN", "EIGHTEEN", "NINETEEN"
};

static const char * const TENS[] = {
    "TWENTY", "THIRTY", "FORTY", "FIFTY",
    "SIXTY", "SEVENTY", "EIGHTY", "NINETY"
};

/* Append the word for a number < 1000 to the buffer. */
static void append_number_part(unsigned int num, char *buf, size_t *p)
{
    if (num >= 100) {
        unsigned int hundreds = num / 100;
        const char *s = UNITS[hundreds - 1];
        size_t l = strlen(s);
        memcpy(buf + *p, s, l); *p += l;
        memcpy(buf + *p, "HUNDRED", 7); *p += 7;
        num %= 100;
    }

    if (num >= 20) {
        unsigned int tens = num / 10;
        const char *s = TENS[tens - 2];
        size_t l = strlen(s);
        memcpy(buf + *p, s, l); *p += l;
        num %= 10;
    }

    if (num >= 10) {
        const char *s = TEENS[num - 10];
        size_t l = strlen(s);
        memcpy(buf + *p, s, l); *p += l;
        num = 0;
    }

    if (num > 0) {
        const char *s = UNITS[num - 1];
        size_t l = strlen(s);
        memcpy(buf + *p, s, l); *p += l;
    }
}

/* Return a heap‑allocated string containing the word for n
   (1 ≤ n).  The returned pointer must be freed by the caller. */
static char *make_word(unsigned long long n)
{
    char tmpbuf[64];
    size_t p = 0;

    if (n >= 1000) {
        unsigned int thousands = (unsigned int)(n / 1000ULL);
        unsigned int remainder = (unsigned int)(n % 1000ULL);
        append_number_part(thousands, tmpbuf, &p);
        memcpy(tmpbuf + p, "THOUSAND", 8); p += 8;
        if (remainder > 0)
            append_number_part(remainder, tmpbuf, &p);
    } else {
        append_number_part((unsigned int)n, tmpbuf, &p);
    }

    tmpbuf[p] = '\0';
    size_t len = p;
    char *res = (char *)malloc(len + 1U);
    if (!res) return NULL;
    memcpy(res, tmpbuf, len + 1U);
    return res;
}

struct Entry {
    char *word;
    idx_t number;
};

static int cmp_entry(const void *a, const void *b)
{
    const struct Entry *ea = (const struct Entry *)a;
    const struct Entry *eb = (const struct Entry *)b;
    return strcmp(ea->word, eb->word);
}

/* ==============================================================
   Main sequence function
   ============================================================== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; the sequence is defined for n≥1 */

    /* Allocate array for entries. */
    struct Entry *entries =
        (struct Entry *)malloc((size_t)n * sizeof(struct Entry));
    if (!entries) return 0; /* allocation failure */

    /* Build words for 1 … n. */
    for (idx_t i = 1; i <= n; ++i) {
        entries[i - 1].number = i;
        entries[i - 1].word   = make_word(i);
        if (!entries[i - 1].word) {
            /* cleanup on failure */
            for (idx_t k = 0; k < i - 1; ++k)
                free(entries[k].word);
            free(entries);
            return 0;
        }
    }

    /* Sort alphabetically. */
    qsort(entries, (size_t)n, sizeof(struct Entry), cmp_entry);

    /* Locate the entry with number == n. */
    seq_t pos = 0;
    for (idx_t i = 0; i < n; ++i) {
        if (entries[i].number == n) {
            pos = (seq_t)(i + 1);   /* 1‑based position */
            break;
        }
    }

    /* Free allocated memory. */
    for (idx_t i = 0; i < n; ++i) free(entries[i].word);
    free(entries);

    return pos;
}
