/*
 * A124172  Triangle in which row n contains values from 1 to n in
 *          alphabetical order (in English).
 * https://oeis.org/A124172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, word
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A124172:
   Find the row r with triangular(r-1) < n ≤ triangular(r),
   sort the numbers 1..r alphabetically (English words),
   and return the element at position n - triangular(r-1) - 1 in that row. */

#include <stdlib.h>
#include <string.h>

/* ---- Helpers for English number names ---------------------------------- */
static const char *const units[] = {
    "zero", "one", "two", "three", "four", "five", "six",
    "seven", "eight", "nine", "ten", "eleven", "twelve",
    "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
    "eighteen", "nineteen"
};

static const char *const tens[] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

#define NAME_BUF_SIZE 64

/* Recursive function to write the English name of n into buf.
   N should be < 1,000,000 for the purposes of this sequence.           */
static void num_to_name(idx_t n, char *buf)
{
    char *p = buf;

    if (n < 20) {                      /* 0-19 */
        strcpy(p, units[n]);
        return;
    }

    if (n < 100) {                     /* 20-99 */
        strcpy(p, tens[n / 10]);
        p += strlen(tens[n / 10]);
        if (n % 10) {
            *p++ = ' ';
            strcpy(p, units[n % 10]);
        }
        return;
    }

    if (n < 1000) {                    /* 100-999 */
        strcpy(p, units[n / 100]);
        p += strlen(units[n / 100]);
        strcpy(p, " hundred");
        p += 8;
        if (n % 100) {
            *p++ = ' ';
            num_to_name(n % 100, p);
        } else {
            *p = '\0';
        }
        return;
    }

    if (n < 1000000) {                 /* 1,000-999,999 */
        num_to_name(n / 1000, p);
        p += strlen(p);                /* move past the thousand part */
        *p++ = ' ';
        strcpy(p, "thousand");
        p += 8;
        if (n % 1000) {
            *p++ = ' ';
            num_to_name(n % 1000, p);
        } else {
            *p = '\0';
        }
        return;
    }

    /* Fallback for extremely large n (not needed for typical inputs) */
    sprintf(buf, "%llu", (unsigned long long)n);
}

/* ---- Triangle utility ---------------------------------------------------- */
static idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* ---- Entry structure for sorting ------------------------------------------------ */
typedef struct {
    idx_t num;
    char  name[NAME_BUF_SIZE];
} Entry;

/* Comparator for qsort using the alphabetical name. */
static int cmp_entries(const void *a, const void *b)
{
    const Entry *ea = (const Entry *)a;
    const Entry *eb = (const Entry *)b;
    return strcmp(ea->name, eb->name);
}

/* ---- Public sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* harmless guard */

    /* Find the row r that contains position n. */
    idx_t r = 1;
    while (triangular(r) < n) r++;

    /* Build an array of numbers 1..r with their English names. */
    Entry *arr = (Entry *)malloc((size_t)r * sizeof(Entry));
    for (idx_t i = 1; i <= r; ++i) {
        arr[i - 1].num = i;
        num_to_name(i, arr[i - 1].name);
    }

    /* Sort the array alphabetically by name. */
    qsort(arr, (size_t)r, sizeof(Entry), cmp_entries);

    /* Determine the offset within the sorted row. */
    idx_t prev_tri = triangular(r - 1);
    idx_t offset   = n - prev_tri - 1;          /* zero‑based index */
    idx_t result   = arr[(size_t)offset].num;

    free(arr);
    return (seq_t)result;
}
