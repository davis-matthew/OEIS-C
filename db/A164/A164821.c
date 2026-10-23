/*
 * A164821  Number of letters in n-th prime (in German).
 * https://oeis.org/A164821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = length of the German word for the n‑th prime (counting
 *            each letter, treating ß and umlauted letters as single
 *            characters). Formula in OEIS notation: a(n)=A007208(A000040(n))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   utf8_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/utf8_len.h"

/* ---------- German number words --------------------------------------- */
static const char * const short_word[20] = {
    "",       "eins",    "zwei",   "drei",    "vier",   "fünf",
    "sechs",  "sieben",  "acht",   "neun",    "zehn",   "elf",
    "zwölf",  "dreizehn","vierzehn","fünfzehn","sechzehn",
    "siebzehn","achtzehn","neunzehn"
};

/* 1–9 for use inside compound numbers (e.g. „zweiundzwanzig“) */
static const char * const units_comp[10] = {
    "",     "ein", "zwei", "drei", "vier", "fünf",
    "sechs","sieben","acht","neun"
};

static const char * const tens_word[10] = {
    "",      "",       "zwanzig", "dreißig", "vierzig",
    "fünfzig","sechzig","siebzig",
    "achtzig","neunzig"
};

/* add := count letters in word w */
static void add_word(const char *w, size_t *len)
{
    *len += utf8_len(w);
}

/* length of the German word for n (0 < n < 1000) or for a thousand part.
   If is_thousand_part, the number 1 is rendered as "ein" instead of "eins". */
static size_t german_len(uint32_t n, int is_thousand_part)
{
    size_t len = 0;
    if (n == 0) return 0;

    if (n == 1 && is_thousand_part) {
        add_word("ein", &len);
        return len;
    }

    uint32_t hundred = n / 100;
    uint32_t rem    = n % 100;
    if (hundred > 0) {
        if (hundred == 1 && rem == 0) {
            add_word("hundert", &len);
        } else if (hundred == 1) {
            add_word("einhundert", &len);
        } else {
            add_word(units_comp[hundred], &len);
            add_word("hundert", &len);
        }
    }

    if (rem > 0) {
        if (rem < 20) {
            add_word(short_word[rem], &len);
        } else {
            uint32_t tens  = rem / 10;
            uint32_t units = rem % 10;
            if (units > 0) {
                add_word(units_comp[units], &len);
                add_word("und", &len);
            }
            add_word(tens_word[tens], &len);
        }
    }

    return len;
}

/* ---------- Prime handling (simple sieve up to a fixed limit) --------- */
static uint32_t *prime_buf = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count != 0) return;          /* already initialized */

    /* 10 000th prime is 104 729, so a limit of 200 000 is safe. */
    const size_t LIMIT = 200000;

    unsigned char *is_comp = (unsigned char *)calloc(LIMIT + 1, 1);
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_comp[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_comp[q] = 1;
        }
    }

    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_buf = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_buf[idx++] = (uint32_t)i;

    free(is_comp);
}

/* ---------- The required sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, but safe */

    init_primes();

    if ((size_t)n > prime_count) return (seq_t)0; /* out of bounds */

    uint32_t prime = prime_buf[(size_t)n - 1];

    size_t len = 0;

    uint32_t thousand = prime / 1000;
    uint32_t rest     = prime % 1000;

    if (thousand > 0) {
        len += german_len(thousand, 1);   /* thousand part */
        len += utf8_len("tausend");
    }
    if (rest > 0) {
        len += german_len(rest, 0);       /* remaining part */
    }

    return (seq_t)len;
}
