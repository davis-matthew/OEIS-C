/*
 * A008538  Numbers whose American English name contains the letter 's'.
 * https://oeis.org/A008538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms by checking each integer and storing those whose
   American English name contains the letter 's' */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Helper: number -> American English words (lowercase, spaces)       */
/* ------------------------------------------------------------------ */

static const char *ones[10] = {
    "", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

static const char *teens[10] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *tens[10] = {
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

typedef struct {
    const char *name;
    uint64_t    value;
} scale_t;

static const scale_t scales[] = {
    { "quintillion", 1000000000000000000ULL },
    { "quadrillion", 1000000000000000ULL   },
    { "trillion",    1000000000000ULL       },
    { "billion",     1000000000ULL          },
    { "million",     1000000ULL             },
    { "thousand",    1000ULL                }
};
static const unsigned NUM_SCALES = sizeof(scales)/sizeof(scales[0]);

/* Append a word to buffer, ensuring a space if needed */
static inline void append_word(char **p, const char *w, size_t *len)
{
    size_t l = strlen(w);
    memcpy(*p, w, l);
    *p += l;
    *len += l;
}

/* Append a space to buffer if buffer is not empty */
static inline void append_space(char **p, size_t *len)
{
    if (*len > 0) {
        **p = ' ';
        (*p)++;
        (*len)++;
    }
}

/* Convert a number < 1000 into words and append to buffer */
static void write_chunk(uint64_t n, char **p, size_t *len)
{
    if (n == 0) return;

    if (n >= 100) {
        uint64_t h = n / 100;
        write_chunk(h, p, len);
        append_space(p, len);
        append_word(p, "hundred", len);
        n %= 100;
        if (n) {
            append_space(p, len);
        }
    }

    if (n >= 20) {
        uint64_t t = n / 10;
        append_word(p, tens[t], len);
        n %= 10;
        if (n) {
            append_space(p, len);
        }
    }
    if (n >= 10 && n < 20) {
        append_word(p, teens[n - 10], len);
        n = 0;
    }
    if (n > 0 && n < 10) {
        append_word(p, ones[n], len);
        n = 0;
    }
}

/* Convert an arbitrary 64-bit unsigned number into words */
static void number_to_words(uint64_t n, char *buf, size_t bufsize)
{
    char *p = buf;
    size_t len = 0;

    if (n == 0) {
        append_word(&p, "zero", &len);
        *p = '\0';
        return;
    }

    for (unsigned i = 0; i < NUM_SCALES; ++i) {
        if (n >= scales[i].value) {
            uint64_t part = n / scales[i].value;
            n %= scales[i].value;
            write_chunk(part, &p, &len);
            append_space(&p, &len);
            append_word(&p, scales[i].name, &len);
            if (n) {
                append_space(&p, &len);
            }
        }
    }
    if (n > 0) {
        write_chunk(n, &p, &len);
    }

    *p = '\0';
}

/* Check whether the English name of n contains letter 's' */
static bool contains_s(uint64_t n)
{
    char buf[512];
    number_to_words(n, buf, sizeof(buf));
    const char *p = buf;
    while (*p) {
        if (*p == 's') return true;
        ++p;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* The memoized sequence function                                    */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for out-of-range indices */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static uint64_t next_candidate = 1;
    while (cache.size() < (size_t)n) {
        if (contains_s(next_candidate)) cache.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)(n - 1)];
#else
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;
    static idx_t  cache_cap  = 0;
    static uint64_t next_candidate = 1;

    while (cache_len < n) {
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1); /* OOM, terminate */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (contains_s(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
#endif
}
