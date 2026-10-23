/*
 * A135411  Numbers n with property that for each single digit d of n, we can
 *          also see the decimal expansion of d+2 as a substring of n.
 * https://oeis.org/A135411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135411 – numbers in which each digit d is followed by
   a substring equal to the decimal representation of d+2 in the
   decimal expansion of the number.
   The function generates the numbers in increasing order and
   stores them in a static cache so that calls with arbitrary
   indices are answered in O(1) once the required terms have been
   generated. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a given unsigned 64‑bit number satisfies
   the property described above.  The function works for any size
   of seq_t because it never manipulates seq_t directly. */
static bool is_valid(uint64_t num)
{
    /* digits[0] holds the least significant digit, digits[len-1]
       the most significant one.  20 digits are enough for a
       64‑bit integer. */
    uint8_t digits[20];
    unsigned int len = 0;

    /* Extract digits in reverse order. */
    uint64_t tmp = num;
    do {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    bool present[10] = { false };
    bool has10       = false;
    bool has11       = false;

    /* Scan in forward order to collect the presence set and to
       detect the substrings "10" and "11". */
    for (int i = (int)len - 1; i >= 0; --i) {
        unsigned int d = digits[i];
        present[d] = true;

        if (i > 0) {
            unsigned int dnext = digits[i - 1];
            if (d == 1 && dnext == 0) has10 = true;
            if (d == 1 && dnext == 1) has11 = true;
        }
    }

    /* Check the required conditions. */
    for (unsigned int d = 0; d <= 7; ++d) {
        if (present[d] && !present[d + 2]) return false;
    }
    if (present[8] && !has10) return false;
    if (present[9] && !has11) return false;

    return true;
}

/* Static state to keep the generated prefix of the sequence. */
static seq_t *cache      = NULL;   /* array holding the terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static uint64_t next_test = 1;     /* next number to examine */

/* Ensure that the cache has room for one more element. */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – terminate the program, as the
               sequence cannot be produced any further. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Main function required by the specification.                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(0) is unused. */
    if (n == 0) return 0;

    /* Return cached value if already computed. */
    if (n <= (idx_t)cache_len) return cache[n - 1];

    /* Grow the cache until it contains at least n terms. */
    while ((idx_t)cache_len < n) {
        if (is_valid(next_test)) {
            grow_cache();
            cache[cache_len++] = (seq_t)next_test;
        }
        ++next_test;
    }

    return cache[n - 1];
}
