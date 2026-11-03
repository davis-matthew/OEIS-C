/*
 * A109382  Levenshtein distance between successive English names of
 *          nonnegative integers, excluding spaces and hyphens.
 * https://oeis.org/A109382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = LD(nameof(n), nameof(n+1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 8934 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: convert n and n+1 to English words (without spaces or hyphens) and
   return their Levenshtein distance. */
#include <string.h>
#include <stdint.h>

/* Append a string to a buffer, moving the pointer. */
static void append_str(char **pp, const char *s) {
    for (; *s; ++s) { **pp = *s; ++(*pp); }
}

/* Compute Levenshtein distance between two null‑terminated strings. */
static seq_t levenshtein(const char *s1, const char *s2) {
    size_t l1 = strlen(s1);
    size_t l2 = strlen(s2);
    static seq_t row0[512], row1[512];
    seq_t *prev = row0, *curr = row1;

    for (size_t j = 0; j <= l2; ++j) prev[j] = (seq_t)j;

    for (size_t i = 1; i <= l1; ++i) {
        curr[0] = (seq_t)i;
        for (size_t j = 1; j <= l2; ++j) {
            seq_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            seq_t del = prev[j] + 1;
            seq_t ins = curr[j - 1] + 1;
            seq_t sub = prev[j - 1] + cost;
            seq_t minv = del;
            if (ins < minv) minv = ins;
            if (sub < minv) minv = sub;
            curr[j] = minv;
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    return prev[l2];
}

/* English words for 0‑19 and common tens */
static const char * const units[] = {
    "ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN",
    "EIGHT","NINE","TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN",
    "FIFTEEN","SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"
};
static const char * const tens[]  = {"","","TWENTY","THIRTY","FORTY",
    "FIFTY","SIXTY","SEVENTY","EIGHTY","NINETY"};

/* Recursive word writing, building a continuous string of the English name. */
static void write_number(uint64_t n, char **pp) {
    if (n >= 1000000000000ULL) { /* TRILLION */
        write_number(n / 1000000000000ULL, pp);
        append_str(pp, "TRILLION");
        n %= 1000000000000ULL;
    }
    if (n >= 1000000000ULL) { /* BILLION */
        write_number(n / 1000000000ULL, pp);
        append_str(pp, "BILLION");
        n %= 1000000000ULL;
    }
    if (n >= 1000000ULL) { /* MILLION */
        write_number(n / 1000000ULL, pp);
        append_str(pp, "MILLION");
        n %= 1000000ULL;
    }
    if (n >= 1000ULL) { /* THOUSAND */
        write_number(n / 1000ULL, pp);
        append_str(pp, "THOUSAND");
        n %= 1000ULL;
    }
    if (n >= 100ULL) { /* HUNDRED */
        write_number(n / 100ULL, pp);
        append_str(pp, "HUNDRED");
        n %= 100ULL;
    }
    if (n >= 20ULL) {
        append_str(pp, tens[n / 10]);
        n %= 10ULL;
    }
    if (n > 0ULL) { /* 1‑19 */
        append_str(pp, units[(size_t)n]);
    }
}

/* Convert a number to its continuous English representation. */
static void number_to_words(uint64_t n, char *out) {
    if (n == 0) {
        *out = 'Z'; *(out + 1) = 'E'; *(out + 2) = 'R'; *(out + 3) = 'O'; *(out + 4) = '\0';
        return;
    }
    char *p = out;
    write_number(n, &p);
    *p = '\0';
}

/* OEIS sequence A109382: Levenshtein distance between successive English names. */
seq_t a(idx_t n) {
    uint64_t cur = (uint64_t)n;
    uint64_t nxt = cur + 1;
    static char buf1[512], buf2[512];
    number_to_words(cur, buf1);
    number_to_words(nxt, buf2);
    return levenshtein(buf1, buf2);
}
