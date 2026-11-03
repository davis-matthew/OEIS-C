/*
 * A068669  Noncomposite numbers in which every substring is noncomposite.
 * https://oeis.org/A068669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: full, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
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
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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

    int len = num_digits(x);
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
                terms = mem_resize(seq_t, terms, cap);
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
