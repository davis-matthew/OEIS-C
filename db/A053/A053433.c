/*
 * A053433  Numbers with distinct digits in alphabetical order (in English).
 * https://oeis.org/A053433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, word, base, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 1022, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A053433 (Numbers with distinct digits in alphabetical order,
 * English words).  The routine enumerates all subsets of the digits 0–9,
 * checks that the names of the digits are in alphabetical order,
 * selects the numbers with no leading zero except the single-digit 0,
 * sorts the resulting numbers numerically, and returns the n‑th element.
 * The sequence is finite (at most 2¹⁰ – 1 = 1023 elements).
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* English names of the digits */
static const char *digit_names[10] = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

/* Comparator for sorting digits according to their English names */
static int cmp_digit(const void *a, const void *b)
{
    int da = *(const int *)a;
    int db = *(const int *)b;
    return strcmp(digit_names[da], digit_names[db]);
}

/* Comparator for sorting seq_t values numerically */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t av = *(const seq_t *)a;
    const seq_t bv = *(const seq_t *)b;
    if (av < bv) return -1;
    if (av > bv) return 1;
    return 0;
}

/* Return the n-th term of A053433 (1‑based).  For n out of range
 * (including n = 0) the function returns 0. */
seq_t a(idx_t n)
{
    static seq_t seqnums[1024];
    static size_t seqcount = 0;
    static bool initialized = false;

    if (!initialized) {
        seqcount = 0;
        /* Enumerate all non‑empty subsets of {0,…,9} */
        for (unsigned mask = 1; mask < (1u << 10); ++mask) {
            int digits[10];
            size_t sz = 0;
            for (int d = 0; d < 10; ++d) {
                if (mask & (1u << d))
                    digits[sz++] = d;
            }

            /* Order the digits by their English names */
            qsort(digits, sz, sizeof(int), cmp_digit);

            /* Skip numbers with a leading zero except 0 itself */
            if (sz > 1 && digits[0] == 0)
                continue;

            /* Build the numeric value */
            seq_t val = 0;
            for (size_t i = 0; i < sz; ++i)
                val = val * 10 + (seq_t)digits[i];

            seqnums[seqcount++] = val;
        }
        /* Sort the collected numbers numerically */
        qsort(seqnums, seqcount, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || (size_t)n > seqcount)
        return 0;
    return seqnums[(size_t)n - 1];
}
