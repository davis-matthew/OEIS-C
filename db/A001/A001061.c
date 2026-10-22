/*
 * A001061  1-, 2-, 3-, ... digit numbers in alphabetical order in German.
 * https://oeis.org/A001061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   Sort all k-digit numbers (0-9, then 10-99, then 100-999, etc.)
 *            alphabetically by German word representation, with ü/ö/ß
 *            normalized for collation
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *ones[] = {
    "null", "eins", "zwei", "drei", "vier",
    "funf", "sechs", "sieben", "acht", "neun"
};

static const char *teens[] = {
    "zehn", "elf", "zwolf", "dreizehn", "vierzehn",
    "funfzehn", "sechzehn", "siebzehn", "achtzehn", "neunzehn"
};

static const char *tens[] = {
    "", "", "zwanzig", "dreissig", "vierzig",
    "funfzig", "sechzig", "siebzig", "achtzig", "neunzig"
};

static void number_to_german(unsigned num, char *buf) {
    if (num < 10) {
        strcpy(buf, ones[num]);
    } else if (num < 20) {
        strcpy(buf, teens[num - 10]);
    } else if (num < 100) {
        unsigned t = num / 10;
        unsigned o = num % 10;
        if (o == 0) {
            strcpy(buf, tens[t]);
        } else {
            sprintf(buf, "%sund%s", ones[o], tens[t]);
        }
    } else if (num < 1000) {
        unsigned h = num / 100;
        unsigned rem = num % 100;
        if (h == 1) {
            strcpy(buf, "einhundert");
        } else {
            sprintf(buf, "%shundert", ones[h]);
        }
        if (rem > 0) {
            char temp[500];
            number_to_german(rem, temp);
            strcat(buf, temp);
        }
    }
}

struct num_pair {
    unsigned num;
    char german[1000];
};

static int compare_pairs(const void *a, const void *b) {
    return strcmp(((struct num_pair *)a)->german, ((struct num_pair *)b)->german);
}

seq_t a(idx_t n) {
    idx_t pos = n;
    unsigned num_digits = 1;
    idx_t cumulative = 0;
    
    for (unsigned d = 1; d <= 10; d++) {
        idx_t group_size = (d == 1) ? 10 : (idx_t)9;
        for (unsigned i = 1; i < d; i++) group_size *= 10;
        
        if (pos <= cumulative + group_size) {
            num_digits = d;
            pos -= cumulative;
            break;
        }
        cumulative += group_size;
    }
    
    unsigned start = (num_digits == 1) ? 0 : 1;
    for (unsigned i = 1; i < num_digits; i++) start *= 10;
    unsigned end = (num_digits == 1) ? 10 : start * 10;
    
    size_t count = end - start;
    struct num_pair *pairs = (struct num_pair *)malloc(count * sizeof(struct num_pair));
    
    for (size_t i = 0; i < count; i++) {
        pairs[i].num = start + (unsigned)i;
        number_to_german(pairs[i].num, pairs[i].german);
    }
    
    qsort(pairs, count, sizeof(struct num_pair), compare_pairs);
    
    seq_t result = (seq_t)pairs[pos - 1].num;
    free(pairs);
    
    return result;
}
