/*
 * A097960  Arrange 1-digit numbers in Spanish in alphabetical order, then
 *          2-digit numbers, then 3-digit numbers, etc.
 * https://oeis.org/A097960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word, base
 * Formula:   Generate terms by sorting numbers in Spanish words
 *            alphabetically within each digit length (1‑digit numbers first,
 *            then 2‑digit numbers, etc.)
 * Range:     u8 n <= 200, u16 n <= 8848, u32 n <= 8848, u64 n <= 8848,
 *            u128 n <= 8848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Structure holding a number and its Spanish word representation */
struct Entry {
    int n;
    char word[64];
};

/* Spanish words for units 0‑9 */
static const char *const units[10] = {
    "cero", "uno", "dos", "tres", "cuatro",
    "cinco", "seis", "siete", "ocho", "nueve"
};

/* Spanish words for 10‑15 */
static const char *const ten_special[6] = {
    "diez", "once", "doce", "trece", "catorce", "quince"
};

/* Spanish words for 16‑19 */
static const char *const teen[4] = {
    "dieciseis", "diecisiete", "dieciocho", "diecinueve"
};

/* Tens words for 20‑90 */
static const char *const tens_map[10] = {
    "", "", "veinte", "treinta", "cuarenta",
    "cincuenta", "sesenta", "setenta", "ochenta", "noventa"
};

/* Powers of ten up to 10^19 (unsigned 64‑bit) */
static const uint64_t pow10[20] = {
    1ULL,
    10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL, 0ULL
};

/* Cache for sorted entries of each digit length */
static struct Entry *entries_cache[20] = {NULL};
static size_t entry_counts[20] = {0};

/* Comparator for qsort on Spanish word */
static int cmp_entry(const void *a, const void *b)
{
    const struct Entry *ea = (const struct Entry *)a;
    const struct Entry *eb = (const struct Entry *)b;
    return strcmp(ea->word, eb->word);
}

/* Convert an integer to its Spanish word representation (up to 4 digits) */
static void num_word(int n, char *buf)
{
    if (n < 10) {
        strcpy(buf, units[n]);
        return;
    }
    if (n < 16) {
        strcpy(buf, ten_special[n - 10]);
        return;
    }
    if (n < 20) {                /* 16‑19 */
        strcpy(buf, teen[n - 16]);
        return;
    }
    if (n < 30) {                /* 20‑29 */
        if (n == 20) {
            strcpy(buf, "veinte");
            return;
        }
        strcpy(buf, "veinti");
        strcat(buf, units[n - 20]);
        return;
    }
    if (n < 100) {               /* 30‑99 */
        int tens = n / 10;
        int unit = n % 10;
        if (unit == 0) {
            strcpy(buf, tens_map[tens]);
            return;
        }
        strcpy(buf, tens_map[tens]);
        strcat(buf, " y ");
        strcat(buf, units[unit]);
        return;
    }
    if (n < 1000) {              /* 100‑999 */
        if (n == 100) {
            strcpy(buf, "cien");
            return;
        }
        int hundreds = n / 100;
        int rem = n % 100;
        const char *hundred_word;
        switch (hundreds) {
            case 1: hundred_word = "ciento"; break;
            case 2: hundred_word = "doscientos"; break;
            case 3: hundred_word = "trescientos"; break;
            case 4: hundred_word = "cuatrocientos"; break;
            case 5: hundred_word = "quinientos"; break;
            case 6: hundred_word = "seiscientos"; break;
            case 7: hundred_word = "setecientos"; break;
            case 8: hundred_word = "ochocientos"; break;
            case 9: hundred_word = "novecientos"; break;
            default: hundred_word = ""; break;
        }
        strcpy(buf, hundred_word);
        if (rem > 0) {
            strcat(buf, " ");
            char sub[64];
            num_word(rem, sub);
            strcat(buf, sub);
        }
        return;
    }
    /* Handle 1000‑9999 */
    int thousands = n / 1000;
    int rem = n % 1000;
    if (thousands == 1) {
        strcpy(buf, "mil");
    } else {
        char sub_unit[64];
        num_word(thousands, sub_unit);
        strcpy(buf, sub_unit);
        strcat(buf, " mil");
    }
    if (rem > 0) {
        strcat(buf, " ");
        char sub[64];
        num_word(rem, sub);
        strcat(buf, sub);
    }
}

/* Ensure entries for digit length L are allocated and sorted */
static void ensure_entries_len(int L)
{
    if (entries_cache[L]) return;
    size_t start = (L == 1) ? 0 : pow10[L - 1];
    size_t count = (L == 1) ? 10 : 9ULL * pow10[L - 1];
    struct Entry *arr = (struct Entry *)malloc(sizeof(struct Entry) * count);
    for (size_t i = 0; i < count; ++i) {
        int val = (int)(start + i);
        arr[i].n = val;
        num_word(val, arr[i].word);
    }
    qsort(arr, count, sizeof(struct Entry), cmp_entry);
    entries_cache[L] = arr;
    entry_counts[L] = count;
}

/* Sequence function required by the evaluator */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined behaviour for n < 1 */

    size_t cumulative = 10;   /* numbers up to 1‑digit */
    size_t count = 10;
    int L = 1;                /* current digit length */

    while (cumulative < n) {
        ++L;
        count = (L == 1) ? 10 : 9ULL * pow10[L - 1];
        cumulative += count;
    }

    size_t prev_cum = cumulative - count;
    size_t offset = (size_t)(n - prev_cum - 1);   /* 0‑based index within length L */

    ensure_entries_len(L);
    return (seq_t)(entries_cache[L][offset].n);
}
