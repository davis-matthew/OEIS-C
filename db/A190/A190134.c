/*
 * A190134  Numbers 1 through 10000 sorted lexicographically in hexadecimal
 *          representation (base 16).
 * https://oeis.org/A190134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A190134: Numbers 1..10000 sorted lexicographically in hexadecimal representation.
   The function builds the sorted list once (on the first call) and returns the n‑th element.
   It works for both integral and arbitrary‑precision seq_t types. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* item used during sorting: the original number and its hex string (max length 4 for 10000). */
typedef struct {
    idx_t   num;      /* original integer value (1..10000) */
    char    hex[5];   /* hex string, null terminated, max length 4 */
} Item;

/* static cache of the sorted sequence */
static seq_t *sorted   = NULL;  /* pointer to array of seq_t values */
static idx_t  sortedlen = 0;    /* number of elements (always 10000) */
static int    initialized = 0;  /* flag */

static int cmp_hex(const void *a, const void *b)
{
    const Item *x = (const Item *)a;
    const Item *y = (const Item *)b;
    return strcmp(x->hex, y->hex);
}

/* ------------------------------------------------------------------ */
/* Build the cache on first use.                                      */
/* ------------------------------------------------------------------ */
static void init_cache(void)
{
    const idx_t N = 10000;
    Item *items = (Item *)malloc(N * sizeof(Item));
    if (!items) return;          /* unlikely */

    for (idx_t i = 0; i < N; ++i) {
        items[i].num = i + 1;
        /* convert (i+1) to hex string, most significant digit first */
        unsigned int v = (unsigned int)(i + 1);   /* values fit in 32 bits */
        char tmp[5];
        int pos = 0;
        do {
            uint8_t d = v & 0xF;
            tmp[pos++] = "0123456789ABCDEF"[d];
            v >>= 4;
        } while (v > 0);
        /* reverse to get correct order */
        for (int j = 0; j < pos; ++j)
            items[i].hex[j] = tmp[pos - 1 - j];
        items[i].hex[pos] = '\0';
    }

    /* sort by hex representation */
    qsort(items, N, sizeof(Item), cmp_hex);

    /* allocate the final sequence array */
    sorted = (seq_t *)malloc(N * sizeof(seq_t));
    if (!sorted) { free(items); return; }
    for (idx_t i = 0; i < N; ++i)
        sorted[i] = (seq_t)items[i].num;

    sortedlen = N;
    free(items);
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Public function required by the benchmark                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init_cache();

    if (n <= 0 || n > sortedlen) return (seq_t)0;  /* out of bounds */
    return sorted[n - 1];
}
