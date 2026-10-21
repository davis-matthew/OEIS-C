/*
 * A068669  Noncomposite numbers in which every substring is noncomposite.
 * https://oeis.org/A068669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_len, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = n-th positive integer whose every decimal substring is either
   1 or a prime.
   The sequence is finite; all terms are below 10^6 (the last one is 3137).
   We pre‑compute all such numbers on the first call and store them in
   a static array for efficient repeated queries.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_len.h"

static seq_t *terms = NULL;
static size_t term_count = 0;
static bool initialized = false;

/* Check whether all decimal substrings of x are 1 or prime. */
static bool good_number(seq_t x)
{
    if (x == (seq_t)1)
        return true;
    if (!isPrimeGeneric(x))
        return false;              /* x must be prime itself */

    int len = digit_len(x);
    if (len == 0)
        return false;              /* safety */

    /* Extract digits: most significant first */
    unsigned char digits[20]; /* sufficient for x < 10^20 */
    seq_t tmp = x;
    for (int i = len - 1; i >= 0; --i) {
        digits[i] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Test every substring */
    for (int i = 0; i < len; ++i) {
        seq_t val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + digits[j];
            if (val == (seq_t)1)
                continue;
            if (!isPrimeGeneric(val))
                return false;
        }
    }
    return true;
}

/* Generate all terms up to a fixed limit on the first call. */
static void generate_terms(void)
{
    if (initialized)
        return;
    initialized = true;

    const idx_t LIMIT = 1000000; /* sufficient for all known terms */
    size_t cap = 0;
    terms = NULL;
    term_count = 0;

    for (idx_t num = 1; num <= LIMIT; ++num) {
        if (good_number((seq_t)num)) {
            if (term_count == cap) {
                cap = cap ? cap * 2 : 4;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)num;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1 */
    generate_terms();
    if (n <= term_count)
        return terms[n - 1];
    return 0;     /* sequence exhausted for larger n */
}
