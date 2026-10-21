/*
 * A059093  Numbers ending with the letter "e" (in English).
 * https://oeis.org/A059093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = base[(n-1)%37] + 100 * floor((n-1)/37) where base
 *            contains the first 37 positive integers whose English
 *            representation ends with the letter 'e'. The base is computed
 *            algorithmically from the English words for numbers 1..99, so no
 *            hard‑coded values are used
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* English words for units and teens */
static const char *const units[10] = {
    "", "one",  "two",   "three", "four", "five",
    "six", "seven", "eight", "nine"
};

static const char *const teen[10] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *const tens[10] = {
    "", "ten",  "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

/* Return true if the English name of n (1 ≤ n < 100) ends with 'e'. */
static bool ends_with_e_small(int n)
{
    if (n < 10) {
        const char *w = units[n];
        return w[0] && w[strlen(w) - 1] == 'e';
    }
    if (n < 20) {
        const char *w = teen[n - 10];
        return w[0] && w[strlen(w) - 1] == 'e';
    }
    int d = n / 10;      /* tens digit (2..9) */
    int u = n % 10;      /* units digit (0..9) */
    if (u == 0) {        /* e.g., "twenty" */
        const char *w = tens[d];
        return w[0] && w[strlen(w) - 1] == 'e';
    }
    /* e.g., "twenty-one" -> check the unit word */
    const char *w = units[u];
    return w[0] && w[strlen(w) - 1] == 'e';
}

/* Compute the first 37 numbers ending with 'e'.  Called only once. */
static void build_base(seq_t *base)
{
    int i = 1;          /* candidate number */
    int cnt = 0;        /* how many values stored */
    while (cnt < 37) {
        if (ends_with_e_small(i)) {
            base[cnt] = (seq_t)i;
            cnt++;
        }
        i++;
    }
}

/* Sequence A059093 */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* Build the base array once */
    static seq_t base[37];
    static bool built = false;
    if (!built) {
        build_base(base);
        built = true;
    }

    /* Compute block and position */
    idx_t block = (n - 1) / 37;   /* floor((n-1)/37)   */
    idx_t pos   = (n - 1) % 37;   /* (n-1) mod 37      */

    /* Result = base[pos] + 100 * block */
    seq_t blockVal = (seq_t)block;
    seq_t result   = base[(size_t)pos] + blockVal * (seq_t)100;

    return result;
}
